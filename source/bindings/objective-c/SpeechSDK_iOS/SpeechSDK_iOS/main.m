//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "speech_factory.h"

int main(int argc, char * argv[]) {
    @autoreleasepool {
        //return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
        __block bool end = false;
        SpeechFactory *factory = [SpeechFactory fromSubscription:@""                                                              AndRegion:@"westus"];
        
        NSBundle *mainBundle = [NSBundle mainBundle];
        NSString *myFile = [mainBundle pathForResource: @"whatstheweatherlike" ofType:           @"wav"];
        NSLog(@"Main bundle path: %@", mainBundle);
        NSLog(@"myFile path: %@", myFile);
        
        NSLog(@"Factory subscription key %@.",factory.subscriptionKey);
        SpeechRecognizer *recognizer = [factory createSpeechRecognizerWithFileInput:myFile];
        // Test1: Use Recognize()
        //SpeechRecognitionResult *result = [recognizer recognize];
        //NSLog(@"Recognition result %@. Status %ld.", result.text, (long)result.recognitionStatus);
        
        // Test2: Use RecognizeAsync() with completion block
        // [recognizer recognizeAsync: ^ (SpeechRecognitionResult *result){
        //    NSLog(@"Recognition result %@. Status %ld.", result.text, (long)result.recognitionStatus);
        //    end = true;
        // }];
        // while (end == false)
        //    [NSThread sleepForTimeInterval:1.0f];
        
        // Test3: Use StartContinuousRecognitionAsync()
        [recognizer addFinalResultEventListener: ^ (SpeechRecognizer * reconizer, SpeechRecognitionResultEventArgs *eventArgs) {
            NSLog(@"Received final result event. SessionId: %@, recognition result:%@. Status %ld.", eventArgs.sessionId, eventArgs.result.text, (long)eventArgs.result.recognitionStatus);
            end = true;
        }];
        [recognizer addIntermediateResultEventListener: ^ (SpeechRecognizer * reconizer, SpeechRecognitionResultEventArgs *eventArgs) {
            NSLog(@"Received interemdiate result event. SessionId: %@, intermediate result:%@.", eventArgs.sessionId, eventArgs.result.text);
        }];
        [recognizer startContinuousRecognition];
        while (end == false)
            [NSThread sleepForTimeInterval:1.0f];
        [recognizer stopContinuousRecognition];
        
        [recognizer close];
    }
}
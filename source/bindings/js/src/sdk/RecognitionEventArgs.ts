//
// copyright (c) Microsoft. All rights reserved.
// See https://aka.ms/csspeech/license201809 for the full license information.
//

import { SessionEventArgs } from "./Exports";

/**
 * Defines payload for session events like Speech Start/End Detected
 * @class
 */
export class RecognitionEventArgs extends SessionEventArgs {
    private privOffset: number;

    /**
     * Creates and initializes an instance of this class.
     * @constructor
     * @param offset The offset.
     * @param sessionId The session id.
     */
    public constructor(offset: number, sessionId?: string) {
        super(sessionId);

        this.privOffset = offset;
    }

    /**
     * Represents the message offset
     * @property
     */
    public get offset(): number {
        return this.privOffset;
    }
}
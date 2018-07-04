//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//

#include "stdafx.h"

#include "microphone.h"
#include "microphone_pump_base.h"

namespace Microsoft {
namespace CognitiveServices {
namespace Speech {
namespace Impl {

using namespace std;

class MicrophonePump : public MicrophonePumpBase
{
public:
    
    MicrophonePump();

    SPX_INTERFACE_MAP_BEGIN()
        SPX_INTERFACE_MAP_ENTRY(ISpxAudioPump)
    SPX_INTERFACE_MAP_END()

private:
    int  Process(const uint8_t* pBuffer, size_t size);

    static void OnInputStateChange(void* pContext, AUDIO_STATE state)
    {
        static_cast<MicrophonePumpBase*>(pContext)->UpdateState(state);
    }

    static int OnInputWrite(void* pContext, uint8_t* pBuffer, size_t size)
    {
        return static_cast<MicrophonePump*>(pContext)->Process(pBuffer, size);
    }    
};

MicrophonePump::MicrophonePump()
{
    auto result = audio_setcallbacks(m_audioHandle, 
                                     NULL, NULL, 
                                     &MicrophonePump::OnInputStateChange, (void*)this, 
                                     &MicrophonePump::OnInputWrite, (void*)this,
                                     NULL, NULL);
    SPX_IFTRUE_THROW_HR(result != AUDIO_RESULT_OK, SPXERR_MIC_ERROR);

    int val = CHANNELS;
    result = audio_set_options(m_audioHandle, "channels", &val);
    SPX_IFTRUE_THROW_HR(result != AUDIO_RESULT_OK, SPXERR_MIC_ERROR);
    val = BITS_PER_SAMPLE;
    result = audio_set_options(m_audioHandle, "bits_per_sample", &val);
    SPX_IFTRUE_THROW_HR(result != AUDIO_RESULT_OK, SPXERR_MIC_ERROR);
    val = SAMPLES_PER_SECOND;
    result = audio_set_options(m_audioHandle, "sample_rate", &val);
    SPX_IFTRUE_THROW_HR(result != AUDIO_RESULT_OK, SPXERR_MIC_ERROR);
}

int MicrophonePump::Process(const uint8_t* pBuffer, size_t size)
{
    int result = 0;
    
    SPX_IFTRUE_THROW_HR(m_sink == nullptr, SPXERR_INVALID_ARG);

    if (pBuffer != nullptr) 
    {
        auto sharedBuffer = SpxAllocSharedAudioBuffer(size);
        memcpy(sharedBuffer.get(), pBuffer, size);
        m_sink->ProcessAudio(sharedBuffer, size);
    }

    return result;
}

shared_ptr<ISpxAudioPump> Microphone::Create()
{
    return std::make_shared<MicrophonePump>();
}

} } } } // Microsoft::CognitiveServices::Speech::Impl
#pragma once
#include <queue>
#include "ispxinterfaces.h"
#include "interface_helpers.h"
#include "service_helpers.h"
#include "property_bag_impl.h"

namespace Microsoft {
namespace CognitiveServices {
namespace Speech {
namespace Impl {

class CSpxSynthesizer :
    public ISpxObjectWithSiteInitImpl<ISpxGenericSite>,
    public ISpxSynthesizer,
    public ISpxSynthesizerEvents,
    public ISpxTtsEngineAdapterSite,
    public ISpxServiceProvider,
    public ISpxGenericSite,
    public ISpxPropertyBagImpl
{
public:

    CSpxSynthesizer();
    virtual ~CSpxSynthesizer();

    SPX_INTERFACE_MAP_BEGIN()
        SPX_INTERFACE_MAP_ENTRY(ISpxObjectWithSite)
        SPX_INTERFACE_MAP_ENTRY(ISpxObjectInit)
        SPX_INTERFACE_MAP_ENTRY(ISpxServiceProvider)
        SPX_INTERFACE_MAP_ENTRY(ISpxSynthesizer)
        SPX_INTERFACE_MAP_ENTRY(ISpxSynthesizerEvents)
        SPX_INTERFACE_MAP_ENTRY(ISpxTtsEngineAdapterSite)
        SPX_INTERFACE_MAP_ENTRY(ISpxGenericSite)
        SPX_INTERFACE_MAP_ENTRY(ISpxNamedProperties)
    SPX_INTERFACE_MAP_END()

    // --- ISpxObjectInit ---

    void Init() override;
    void Term() override;

    // --- ISpxSynthesizer ---

    bool IsEnabled() override;
    void Enable() override;
    void Disable() override;

    void SetOutput(std::shared_ptr<ISpxAudioOutput> output) override;
    std::shared_ptr<ISpxSynthesisResult> Speak(const std::string& text, bool isSsml) override;
    CSpxAsyncOp<std::shared_ptr<ISpxSynthesisResult>> SpeakAsync(const std::string& text, bool isSsml) override;
    std::shared_ptr<ISpxSynthesisResult> StartSpeaking(const std::string& text, bool isSsml) override;
    CSpxAsyncOp<std::shared_ptr<ISpxSynthesisResult>> StartSpeakingAsync(const std::string& text, bool isSsml) override;

    void Close() override;

    // --- ISpxSynthesizerEvents ---

    void FireSynthesisStarted(std::shared_ptr<ISpxSynthesisResult> result) override;
    void FireSynthesizing(std::shared_ptr<ISpxSynthesisResult> result) override;
    void FireSynthesisCompleted(std::shared_ptr<ISpxSynthesisResult> result) override;
    void FireSynthesisCanceled(std::shared_ptr<ISpxSynthesisResult> result) override;


    // --- ISpxServiceProvider ---

    SPX_SERVICE_MAP_BEGIN()
        SPX_SERVICE_MAP_ENTRY(ISpxNamedProperties)
        SPX_SERVICE_MAP_ENTRY_SITE(GetSite())
    SPX_SERVICE_MAP_END()

    // --- ISpxTtsEngineAdapterSite ---

    uint32_t Write(ISpxTtsEngineAdapter* adapter, const std::wstring& requestId, uint8_t* buffer, uint32_t size) override;


protected:

    std::shared_ptr<ISpxNamedProperties> GetParentProperties() const override;


private:

    void PushRequestIntoQueue(const std::wstring& requestId);
    void WaitUntilRequestInFrontOfQueue(const std::wstring& requestId);
    void PopRequestFromQueue();

    std::shared_ptr<ISpxSynthesisResult> CreateResult(const std::wstring& requestId, ResultReason reason, uint8_t* audio_buffer, size_t audio_length);
    void FireResultEvent(std::shared_ptr<ISpxSynthesisResult> result);
    void FireEvent(std::list<std::pair<void*, std::shared_ptr<SynthEvent_Type>>> events, std::shared_ptr<ISpxSynthesisResult> result);

    void EnsureTtsEngineAdapter();
    void InitializeTtsEngineAdapter();
    void ReleaseTtsEngineAdapter();


private:

    CSpxSynthesizer(const CSpxSynthesizer&) = delete;
    CSpxSynthesizer(const CSpxSynthesizer&&) = delete;

    CSpxSynthesizer& operator=(const CSpxSynthesizer&) = delete;

    std::atomic_bool m_fEnabled;

    std::shared_ptr<ISpxTtsEngineAdapter> m_ttsAdapter;
    std::shared_ptr<ISpxAudioOutput> m_audioOutput;

    std::shared_ptr<ISpxGenericSite> m_siteKeepAlive;

    std::queue<std::wstring> m_requestQueue;
    std::mutex m_queueOperationMutex;
    std::mutex m_requestWaitingMutex;
    std::condition_variable m_cv;
};


} } } } // Microsoft::CognitiveServices::Speech::Impl
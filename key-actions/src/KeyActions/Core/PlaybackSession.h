#pragma once

#include "Recording.h"

#include "Lumina/Input/GlobalInputPlayback.h"

#include <memory>
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>

namespace KeyActions
{
    using PlaybackProgressCallback = std::function<void(float progress, size_t currentEventIndex)>;
    using PlaybackCompleteCallback = std::function<void()>;

    // Playback configuration
    struct PlaybackSettings
    {
        float Speed = 1.0f;              // Playback speed multiplier (0.1x - 5.0x)
        bool Loop = false;               // Loop the recording continuously
        bool IgnoreMouseMove = false;    // Skip mouse movement events
        int StartFromIndex = 0;          // Start from specific event index
        int StopAtIndex = -1;            // Stop at specific event index (-1 = play to end)
    };

    class PlaybackSession
    {
    public:
        PlaybackSession();
        ~PlaybackSession();

        // Playback control
        bool Play(const Recording& recording, const PlaybackSettings& settings = PlaybackSettings());
        void Stop();
        void Pause();
        void Resume();

        // State queries
        bool IsPlaying() const { return m_IsPlaying; }
        bool IsPaused() const { return m_IsPaused; }
        float GetProgress() const;
        size_t GetCurrentEventIndex() const { return m_CurrentEventIndex; }
        size_t GetTotalEvents() const { return m_TotalEvents; }
        float GetElapsedTime() const { return m_CurrentTime; }
        float GetTotalDuration() const { return m_TotalDuration; }

        // Callbacks
        void SetProgressCallback(PlaybackProgressCallback callback);
        void SetCompleteCallback(PlaybackCompleteCallback callback);

    private:
        void PlaybackThread(Recording recording, PlaybackSettings settings);
        void SimulateEvent(const RecordedEvent& event);

        std::unique_ptr<Lumina::GlobalInputPlayback> m_Playback;
        std::thread m_PlaybackThread;
        std::mutex m_CallbackMutex;

        std::atomic<bool> m_IsPlaying{ false };
        std::atomic<bool> m_IsPaused{ false };
        std::atomic<bool> m_ShouldStop{ false };
        std::atomic<float> m_CurrentTime{ 0.0f };
        std::atomic<size_t> m_CurrentEventIndex{ 0 };
        std::atomic<size_t> m_TotalEvents{ 0 };
        float m_TotalDuration{ 0.0f };

        PlaybackProgressCallback m_ProgressCallback;
        PlaybackCompleteCallback m_CompleteCallback;
    };
}
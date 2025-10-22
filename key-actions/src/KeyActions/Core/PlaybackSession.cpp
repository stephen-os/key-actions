#include "PlaybackSession.h"

#include "Lumina/Core/Application.h"
#include "Lumina/Core/Log.h"

#include <chrono>

namespace KeyActions
{
    PlaybackSession::PlaybackSession()
    {
        m_Playback = Lumina::GlobalInputPlayback::Create();

        if (!m_Playback)
        {
            LUMINA_LOG_ERROR("Failed to create GlobalInputPlayback - platform not supported");
        }
    }

    PlaybackSession::~PlaybackSession()
    {
        Stop();

        if (m_PlaybackThread.joinable())
        {
            m_PlaybackThread.join();
        }
    }

    bool PlaybackSession::Play(const Recording& recording, const PlaybackSettings& settings)
    {
        if (!m_Playback)
        {
            LUMINA_LOG_ERROR("GlobalInputPlayback not available");
            return false;
        }

        if (m_IsPlaying)
        {
            LUMINA_LOG_WARN("Already playing a recording");
            return false;
        }

        if (recording.Events.empty())
        {
            LUMINA_LOG_WARN("Cannot play empty recording");
            return false;
        }

        // Stop any existing playback thread
        if (m_PlaybackThread.joinable())
        {
            m_PlaybackThread.join();
        }

        m_IsPlaying = true;
        m_IsPaused = false;
        m_ShouldStop = false;
        m_CurrentTime = 0.0f;
        m_CurrentEventIndex = 0;
        m_TotalEvents = recording.Events.size();
        m_TotalDuration = recording.TotalDuration;

        // Start playback on separate thread
        m_PlaybackThread = std::thread(&PlaybackSession::PlaybackThread, this, recording, settings);

        LUMINA_LOG_INFO("Started playback of recording: {}", recording.Name);
        return true;
    }

    void PlaybackSession::Stop()
    {
        if (!m_IsPlaying)
            return;

        m_ShouldStop = true;
        m_IsPlaying = false;
        m_IsPaused = false;

        if (m_PlaybackThread.joinable())
        {
            m_PlaybackThread.join();
        }

        LUMINA_LOG_INFO("Stopped playback");
    }

    void PlaybackSession::Pause()
    {
        if (m_IsPlaying && !m_IsPaused)
        {
            m_IsPaused = true;
            LUMINA_LOG_INFO("Paused playback");
        }
    }

    void PlaybackSession::Resume()
    {
        if (m_IsPlaying && m_IsPaused)
        {
            m_IsPaused = false;
            LUMINA_LOG_INFO("Resumed playback");
        }
    }

    float PlaybackSession::GetProgress() const
    {
        if (m_TotalDuration <= 0.0f)
            return 0.0f;

        return std::min(1.0f, m_CurrentTime / m_TotalDuration);
    }

    void PlaybackSession::SetProgressCallback(PlaybackProgressCallback callback)
    {
        std::lock_guard<std::mutex> lock(m_CallbackMutex);
        m_ProgressCallback = callback;
    }

    void PlaybackSession::SetCompleteCallback(PlaybackCompleteCallback callback)
    {
        std::lock_guard<std::mutex> lock(m_CallbackMutex);
        m_CompleteCallback = callback;
    }

    void PlaybackSession::PlaybackThread(Recording recording, PlaybackSettings settings)
    {
        using namespace std::chrono;

        auto startTime = high_resolution_clock::now();
        auto pauseStartTime = startTime;
        duration<float> totalPausedTime = duration<float>::zero();

        do // Loop if settings.Loop is true
        {
            int startIndex = std::max(0, settings.StartFromIndex);
            int endIndex = settings.StopAtIndex >= 0 ?
                std::min(settings.StopAtIndex, (int)recording.Events.size() - 1) :
                recording.Events.size() - 1;

            for (int i = startIndex; i <= endIndex && !m_ShouldStop; ++i)
            {
                const auto& event = recording.Events[i];

                // Skip mouse moves if requested
                if (settings.IgnoreMouseMove && event.Action == RecordedAction::MouseMoved)
                    continue;

                // Handle pause
                while (m_IsPaused && !m_ShouldStop)
                {
                    if (pauseStartTime == startTime) // First time pausing
                        pauseStartTime = high_resolution_clock::now();

                    std::this_thread::sleep_for(milliseconds(10));
                }

                // Account for paused time
                if (pauseStartTime != startTime)
                {
                    auto pauseEndTime = high_resolution_clock::now();
                    totalPausedTime += duration_cast<duration<float>>(pauseEndTime - pauseStartTime);
                    pauseStartTime = startTime; // Reset
                }

                // Calculate target time for this event
                float targetTime = event.Time / settings.Speed;

                // Wait until it's time to play this event
                while (!m_ShouldStop)
                {
                    auto currentTime = high_resolution_clock::now();
                    auto elapsed = duration_cast<duration<float>>(currentTime - startTime) - totalPausedTime;

                    m_CurrentTime = elapsed.count();

                    if (elapsed.count() >= targetTime)
                        break;

                    std::this_thread::sleep_for(milliseconds(1));
                }

                if (m_ShouldStop)
                    break;

                // Simulate the event
                SimulateEvent(event);

                m_CurrentEventIndex = i;

                // Call progress callback if set
                {
                    std::lock_guard<std::mutex> lock(m_CallbackMutex);
                    if (m_ProgressCallback)
                    {
                        float progress = m_TotalDuration > 0.0f ? (m_CurrentTime / m_TotalDuration) : 0.0f;
                        m_ProgressCallback(progress, i);
                    }
                }
            }

            // Reset for loop
            if (settings.Loop && !m_ShouldStop)
            {
                startTime = high_resolution_clock::now();
                totalPausedTime = duration<float>::zero();
                m_CurrentTime = 0.0f;
                m_CurrentEventIndex = 0;
            }

        } while (settings.Loop && !m_ShouldStop);

        m_IsPlaying = false;

        // Call complete callback
        {
            std::lock_guard<std::mutex> lock(m_CallbackMutex);
            if (m_CompleteCallback)
            {
                m_CompleteCallback();
            }
        }

        LUMINA_LOG_INFO("Playback completed");
    }

    void PlaybackSession::SimulateEvent(const RecordedEvent& event)
    {
        switch (event.Action)
        {
        case RecordedAction::KeyPressed:
            m_Playback->SimulateKeyPress(event.Key);
            break;

        case RecordedAction::KeyReleased:
            m_Playback->SimulateKeyRelease(event.Key);
            break;

        case RecordedAction::MousePressed:
            m_Playback->SimulateMouseButtonPress(event.Button, event.MouseX, event.MouseY);
            break;

        case RecordedAction::MouseReleased:
            m_Playback->SimulateMouseButtonRelease(event.Button, event.MouseX, event.MouseY);
            break;

        case RecordedAction::MouseMoved:
            m_Playback->SimulateMouseMove(event.MouseX, event.MouseY);
            break;

        case RecordedAction::MouseScrolled:
            m_Playback->SimulateMouseScroll(event.ScrollDX, event.ScrollDY);
            break;
        }
    }
}
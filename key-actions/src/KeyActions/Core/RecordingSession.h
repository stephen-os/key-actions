#pragma once

#include "Recording.h"

#include "Lumina/Input/GlobalInputCapture.h"

#include <memory>
#include <functional>
#include <string>

namespace Lumina
{
    // Forward declarations
    class GlobalKeyPressedEvent;
    class GlobalKeyReleasedEvent;
    class GlobalMouseButtonPressedEvent;
    class GlobalMouseButtonReleasedEvent;
    class GlobalMouseMovedEvent;
    class GlobalMouseScrolledEvent;

    // Callbacks for recording events
    using RecordingEventCallback = std::function<void(const RecordedEvent& event)>;
    using RecordingStateCallback = std::function<void()>;

    // Recording configuration
    struct RecordingSettings
    {
        std::string Name;
        bool RecordMouseMovement = false;
        int InitialDelaySeconds = 0;
        float MouseMoveThreshold = 0.02f;  // Seconds between mouse move events
    };

    // Manages recording of input events
    class RecordingSession
    {
    public:
        RecordingSession();
        ~RecordingSession();

        // Recording control
        bool Start(const RecordingSettings& settings);
        void Stop();
        void Cancel();

        // State queries
        bool IsRecording() const { return m_IsRecording; }
        bool IsWaitingForDelay() const { return m_IsWaitingForDelay; }
        float GetDelayTimeRemaining() const { return m_DelayTimer; }
        float GetElapsedTime() const;
        size_t GetEventCount() const;

        // Get the current recording (while recording or after stopped)
        const Recording& GetRecording() const { return m_CurrentRecording; }
        Recording TakeRecording(); // Move ownership of recording

        // Update (call every frame)
        void Update(float timestep);

        // Event handling (call from your event dispatcher)
        void OnGlobalKeyPressed(GlobalKeyPressedEvent& e);
        void OnGlobalKeyReleased(GlobalKeyReleasedEvent& e);
        void OnGlobalMouseButtonPressed(GlobalMouseButtonPressedEvent& e);
        void OnGlobalMouseButtonReleased(GlobalMouseButtonReleasedEvent& e);
        void OnGlobalMouseMoved(GlobalMouseMovedEvent& e);
        void OnGlobalMouseScrolled(GlobalMouseScrolledEvent& e);

        // Callbacks
        void SetEventRecordedCallback(RecordingEventCallback callback);
        void SetRecordingStartedCallback(RecordingStateCallback callback);
        void SetRecordingStoppedCallback(RecordingStateCallback callback);

    private:
        std::unique_ptr<GlobalInputCapture> m_GlobalInputCapture;

        // Recording state
        bool m_IsRecording = false;
        bool m_IsWaitingForDelay = false;
        float m_DelayTimer = 0.0f;
        float m_RecordingStartTime = 0.0f;
        float m_LastMouseMoveTime = 0.0f;

        // Current recording
        Recording m_CurrentRecording;
        RecordingSettings m_Settings;

        // Callbacks
        RecordingEventCallback m_EventRecordedCallback;
        RecordingStateCallback m_RecordingStartedCallback;
        RecordingStateCallback m_RecordingStoppedCallback;
    };
}
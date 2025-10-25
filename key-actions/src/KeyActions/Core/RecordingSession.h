#pragma once

#include "Recording.h"

#include "Lumina/Events/GlobalKeyEvent.h"
#include "Lumina/Events/GlobalMouseEvent.h"

#include <memory>
#include <functional>
#include <string>

namespace KeyActions
{
    struct RecordingSettings
    {
        std::string Name;
        bool RecordMouseMovement = false;
        int InitialDelaySeconds = 0;
        float MouseMoveThreshold = 0.02f;
    };

    class RecordingSession
    {
    public:
        using KeyPressedEvent = Lumina::GlobalKeyPressedEvent;
        using KeyReleasedEvent = Lumina::GlobalKeyReleasedEvent;
        
        using MouseButtonPressedEvent = Lumina::GlobalMouseButtonPressedEvent;
        using MouseButtonReleasedEvent = Lumina::GlobalMouseButtonReleasedEvent;
        using MouseMovedEvent = Lumina::GlobalMouseMovedEvent;
		using MouseScrolledEvent = Lumina::GlobalMouseScrolledEvent;

        using RecordingEventCallback = std::function<void(const RecordedEvent& event)>;
        using RecordingStateCallback = std::function<void()>;


        RecordingSession();
        ~RecordingSession();

        // Recording control
        bool Start(const RecordingSettings& settings);
        void Stop();
        void Cancel();

        bool IsRecording() const { return m_IsRecording; }
        bool IsWaitingForDelay() const { return m_IsWaitingForDelay; }
        float GetDelayTimeRemaining() const { return m_DelayTimer; }
        float GetElapsedTime() const;
        size_t GetEventCount() const;

        const Recording& GetRecording() const { return m_CurrentRecording; }
        Recording TakeRecording();

        void Update(float timestep);

        void OnKeyPressed(KeyPressedEvent& e);
        void OnKeyReleased(KeyReleasedEvent& e);
        void OnMouseButtonPressed(MouseButtonPressedEvent& e);
        void OnMouseButtonReleased(MouseButtonReleasedEvent& e);
        void OnMouseMoved(MouseMovedEvent& e);
        void OnMouseScrolled(MouseScrolledEvent& e);

        void SetEventRecordedCallback(RecordingEventCallback callback);
        void SetRecordingStartedCallback(RecordingStateCallback callback);
        void SetRecordingStoppedCallback(RecordingStateCallback callback);

    private:
        bool m_IsRecording = false;
        bool m_IsWaitingForDelay = false;
        float m_DelayTimer = 0.0f;
        float m_RecordingStartTime = 0.0f;
        float m_LastMouseMoveTime = 0.0f;

        Recording m_CurrentRecording;
        RecordingSettings m_Settings;

        RecordingEventCallback m_EventRecordedCallback;
        RecordingStateCallback m_RecordingStartedCallback;
        RecordingStateCallback m_RecordingStoppedCallback;
    };
}
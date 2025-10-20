#include "RecordingSession.h"
#include "Lumina/Core/Application.h"
#include "Lumina/Core/Input.h"
#include "Lumina/Core/Log.h"
#include "Lumina/Events/GlobalKeyEvent.h"
#include "Lumina/Events/GlobalMouseEvent.h"

namespace Lumina
{
    RecordingSession::RecordingSession()
    {
        m_GlobalInputCapture = GlobalInputCapture::Create();

        if (!m_GlobalInputCapture)
        {
            LUMINA_LOG_ERROR("Failed to create GlobalInputCapture - platform not supported");
        }
    }

    RecordingSession::~RecordingSession()
    {
        if (m_IsRecording)
        {
            Stop();
        }

        if (m_GlobalInputCapture && m_GlobalInputCapture->IsActive())
        {
            m_GlobalInputCapture->Stop();
        }
    }

    bool RecordingSession::Start(const RecordingSettings& settings)
    {
        if (!m_GlobalInputCapture)
        {
            LUMINA_LOG_ERROR("GlobalInputCapture not available");
            return false;
        }

        if (m_IsRecording || m_IsWaitingForDelay)
        {
            LUMINA_LOG_WARN("Already recording");
            return false;
        }

        if (settings.Name.empty())
        {
            LUMINA_LOG_WARN("Recording name cannot be empty");
            return false;
        }

        // Store settings
        m_Settings = settings;

        // Create new recording
        m_CurrentRecording = Recording(settings.Name, settings.RecordMouseMovement);

        // Enable event posting to application
        m_GlobalInputCapture->SetPostEventsToApplication(true);

        // Start global input capture
        if (!m_GlobalInputCapture->Start())
        {
            LUMINA_LOG_ERROR("Failed to start global input capture");
            return false;
        }

        // Handle initial delay
        if (settings.InitialDelaySeconds > 0)
        {
            m_IsWaitingForDelay = true;
            m_DelayTimer = static_cast<float>(settings.InitialDelaySeconds);
            LUMINA_LOG_INFO("Starting recording with {} second delay...", settings.InitialDelaySeconds);
        }
        else
        {
            m_IsRecording = true;
            m_RecordingStartTime = Application::GetTime();
            LUMINA_LOG_INFO("Recording started: {}", settings.Name);

            if (m_RecordingStartedCallback)
            {
                m_RecordingStartedCallback();
            }
        }

        return true;
    }

    void RecordingSession::Stop()
    {
        if (!m_IsRecording && !m_IsWaitingForDelay)
            return;

        if (m_IsWaitingForDelay)
        {
            m_IsWaitingForDelay = false;
            LUMINA_LOG_INFO("Recording cancelled (was waiting for delay)");

            if (m_GlobalInputCapture)
            {
                m_GlobalInputCapture->Stop();
            }
            return;
        }

        m_IsRecording = false;
        m_CurrentRecording.TotalDuration = Application::GetTime() - m_RecordingStartTime;

        // Stop global input capture
        if (m_GlobalInputCapture)
        {
            m_GlobalInputCapture->Stop();
        }

        LUMINA_LOG_INFO("Recording stopped: {} (Duration: {}s, Events: {})",
            m_CurrentRecording.Name,
            m_CurrentRecording.TotalDuration,
            m_CurrentRecording.Events.size());

        if (m_RecordingStoppedCallback)
        {
            m_RecordingStoppedCallback();
        }
    }

    void RecordingSession::Cancel()
    {
        if (!m_IsRecording && !m_IsWaitingForDelay)
            return;

        m_IsRecording = false;
        m_IsWaitingForDelay = false;

        if (m_GlobalInputCapture)
        {
            m_GlobalInputCapture->Stop();
        }

        m_CurrentRecording = Recording(); // Clear recording

        LUMINA_LOG_INFO("Recording cancelled");
    }

    float RecordingSession::GetElapsedTime() const
    {
        if (!m_IsRecording)
            return 0.0f;

        return Application::GetTime() - m_RecordingStartTime;
    }

    size_t RecordingSession::GetEventCount() const
    {
        return m_CurrentRecording.Events.size();
    }

    Recording RecordingSession::TakeRecording()
    {
        Recording result = std::move(m_CurrentRecording);
        m_CurrentRecording = Recording();
        return result;
    }

    void RecordingSession::Update(float timestep)
    {
        if (m_IsWaitingForDelay)
        {
            m_DelayTimer -= timestep;
            if (m_DelayTimer <= 0.0f)
            {
                m_IsWaitingForDelay = false;
                m_IsRecording = true;
                m_RecordingStartTime = Application::GetTime();
                LUMINA_LOG_INFO("Recording started: {}", m_Settings.Name);

                if (m_RecordingStartedCallback)
                {
                    m_RecordingStartedCallback();
                }
            }
        }
    }

    void RecordingSession::SetEventRecordedCallback(RecordingEventCallback callback)
    {
        m_EventRecordedCallback = callback;
    }

    void RecordingSession::SetRecordingStartedCallback(RecordingStateCallback callback)
    {
        m_RecordingStartedCallback = callback;
    }

    void RecordingSession::SetRecordingStoppedCallback(RecordingStateCallback callback)
    {
        m_RecordingStoppedCallback = callback;
    }

    void RecordingSession::OnGlobalKeyPressed(GlobalKeyPressedEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::KeyPressed;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Key = e.GetKeyCode();

        // Capture modifier states
        event.ShiftPressed = Input::IsShiftPressed();
        event.CtrlPressed = Input::IsCtrlPressed();
        event.AltPressed = Input::IsAltPressed();
        event.SuperPressed = Input::IsSuperPressed();
        event.CapsLockActive = Input::IsCapsLockActive();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }

    void RecordingSession::OnGlobalKeyReleased(GlobalKeyReleasedEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::KeyReleased;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Key = e.GetKeyCode();

        // Capture modifier states
        event.ShiftPressed = Input::IsShiftPressed();
        event.CtrlPressed = Input::IsCtrlPressed();
        event.AltPressed = Input::IsAltPressed();
        event.SuperPressed = Input::IsSuperPressed();
        event.CapsLockActive = Input::IsCapsLockActive();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }

    void RecordingSession::OnGlobalMouseButtonPressed(GlobalMouseButtonPressedEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::MousePressed;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = e.GetX();
        event.MouseY = e.GetY();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }

    void RecordingSession::OnGlobalMouseButtonReleased(GlobalMouseButtonReleasedEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::MouseReleased;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = e.GetX();
        event.MouseY = e.GetY();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }

    void RecordingSession::OnGlobalMouseMoved(GlobalMouseMovedEvent& e)
    {
        if (!m_IsRecording)
            return;

        float currentTime = Application::GetTime();

        // Throttle mouse moves
        if (currentTime - m_LastMouseMoveTime < m_Settings.MouseMoveThreshold)
            return;

        m_LastMouseMoveTime = currentTime;

        RecordedEvent event;
        event.Action = RecordedAction::MouseMoved;
        event.Time = currentTime - m_RecordingStartTime;
        event.MouseX = e.GetX();
        event.MouseY = e.GetY();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }

    void RecordingSession::OnGlobalMouseScrolled(GlobalMouseScrolledEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::MouseScrolled;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.ScrollDX = e.GetDX();
        event.ScrollDY = e.GetDY();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }
}
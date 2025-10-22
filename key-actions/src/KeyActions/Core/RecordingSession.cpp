#include "RecordingSession.h"

#include "Lumina/Core/Application.h"
#include "Lumina/Core/Input.h"
#include "Lumina/Core/Log.h"

namespace KeyActions
{
    RecordingSession::RecordingSession()
    {
        m_GlobalInputCapture = Lumina::GlobalInputCapture::Create();

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

        m_Settings = settings;

        m_CurrentRecording = Recording(settings.Name, settings.RecordMouseMovement);

        m_GlobalInputCapture->SetPostEventsToApplication(true);

        if (!m_GlobalInputCapture->Start())
        {
            LUMINA_LOG_ERROR("Failed to start global input capture");
            return false;
        }

        if (settings.InitialDelaySeconds > 0)
        {
            m_IsWaitingForDelay = true;
            m_DelayTimer = static_cast<float>(settings.InitialDelaySeconds);
            LUMINA_LOG_INFO("Starting recording with {} second delay...", settings.InitialDelaySeconds);
        }
        else
        {
            m_IsRecording = true;
            m_RecordingStartTime = Lumina::Application::GetTime();
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
        m_CurrentRecording.TotalDuration = Lumina::Application::GetTime() - m_RecordingStartTime;

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

        m_CurrentRecording = Recording();

        LUMINA_LOG_INFO("Recording cancelled");
    }

    float RecordingSession::GetElapsedTime() const
    {
        if (!m_IsRecording)
            return 0.0f;

        return Lumina::Application::GetTime() - m_RecordingStartTime;
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
                m_RecordingStartTime = Lumina::Application::GetTime();
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

    void RecordingSession::OnKeyPressed(KeyPressedEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::KeyPressed;
        event.Time = Lumina::Application::GetTime() - m_RecordingStartTime;
        event.Key = e.GetKeyCode();

        // Capture modifier states
        event.ShiftPressed = Lumina::Input::IsShiftPressed();
        event.CtrlPressed = Lumina::Input::IsCtrlPressed();
        event.AltPressed = Lumina::Input::IsAltPressed();
        event.SuperPressed = Lumina::Input::IsSuperPressed();
        event.CapsLockActive = Lumina::Input::IsCapsLockActive();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }

    void RecordingSession::OnKeyReleased(KeyReleasedEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::KeyReleased;
        event.Time = Lumina::Application::GetTime() - m_RecordingStartTime;
        event.Key = e.GetKeyCode();

        event.ShiftPressed = Lumina::Input::IsShiftPressed();
        event.CtrlPressed = Lumina::Input::IsCtrlPressed();
        event.AltPressed = Lumina::Input::IsAltPressed();
        event.SuperPressed = Lumina::Input::IsSuperPressed();
        event.CapsLockActive = Lumina::Input::IsCapsLockActive();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }

    void RecordingSession::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::MousePressed;
        event.Time = Lumina::Application::GetTime() - m_RecordingStartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = e.GetX();
        event.MouseY = e.GetY();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }

    void RecordingSession::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::MouseReleased;
        event.Time = Lumina::Application::GetTime() - m_RecordingStartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = e.GetX();
        event.MouseY = e.GetY();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }

    void RecordingSession::OnMouseMoved(MouseMovedEvent& e)
    {
        if (!m_IsRecording)
            return;

        float currentTime = Lumina::Application::GetTime();

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

    void RecordingSession::OnMouseScrolled(MouseScrolledEvent& e)
    {
        if (!m_IsRecording)
            return;

        RecordedEvent event;
        event.Action = RecordedAction::MouseScrolled;
        event.Time = Lumina::Application::GetTime() - m_RecordingStartTime;
        event.ScrollDX = e.GetDX();
        event.ScrollDY = e.GetDY();

        m_CurrentRecording.Events.push_back(event);

        if (m_EventRecordedCallback)
        {
            m_EventRecordedCallback(event);
        }
    }
}
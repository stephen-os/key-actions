#include "RecordingTab.h"
#include "Core/Serialization.h"
#include "Lumina/Core/Application.h"
#include "Lumina/Core/Input.h"
#include "Lumina/Core/Log.h"
#include "Lumina/Events/GlobalKeyEvent.h"
#include "Lumina/Events/GlobalMouseEvent.h"
#include <cstring>

namespace Lumina
{
    RecordingTab::RecordingTab()
        : Tab("Recording"), m_EventPanel(1000)
    {
    }

    void RecordingTab::OnAttach()
    {
        // Create global input system
        m_GlobalInputCapture = GlobalInputCapture::Create();

        if (!m_GlobalInputCapture)
        {
            LUMINA_LOG_ERROR("Failed to create GlobalInput - platform not supported");
        }

        LUMINA_LOG_INFO("Recording tab initialized");
    }

    void RecordingTab::OnDetach()
    {
        if (m_IsRecording)
            StopRecording();

        if (m_GlobalInputCapture && m_GlobalInputCapture->IsActive())
        {
            m_GlobalInputCapture->Stop();
        }
    }

    void RecordingTab::OnUpdate(float timestep)
    {
        if (m_IsWaitingForDelay)
        {
            m_DelayTimer -= timestep;
            if (m_DelayTimer <= 0.0f)
            {
                m_IsWaitingForDelay = false;
                m_IsRecording = true;
                m_RecordingStartTime = Application::GetTime();

                RecordedEvent event;
                event.Action = RecordedAction::KeyPressed;
                event.Time = 0.0f;
                event.Key = KeyCode::Unknown;
                LUMINA_LOG_INFO("Recording started!");
            }
        }
    }

    void RecordingTab::OnEvent(Event& e)
    {
        if (!m_IsRecording)
            return;

        EventDispatcher dispatcher(e);

        // Handle global events
        dispatcher.Dispatch<GlobalKeyPressedEvent>([this](GlobalKeyPressedEvent& event) {
            return OnGlobalKeyPressed(event);
            });

        dispatcher.Dispatch<GlobalKeyReleasedEvent>([this](GlobalKeyReleasedEvent& event) {
            return OnGlobalKeyReleased(event);
            });

        dispatcher.Dispatch<GlobalMouseButtonPressedEvent>([this](GlobalMouseButtonPressedEvent& event) {
            return OnGlobalMouseButtonPressed(event);
            });

        dispatcher.Dispatch<GlobalMouseButtonReleasedEvent>([this](GlobalMouseButtonReleasedEvent& event) {
            return OnGlobalMouseButtonReleased(event);
            });

        if (m_RecordMouseMovement)
        {
            dispatcher.Dispatch<GlobalMouseMovedEvent>([this](GlobalMouseMovedEvent& event) {
                return OnGlobalMouseMoved(event);
                });
        }

        dispatcher.Dispatch<GlobalMouseScrolledEvent>([this](GlobalMouseScrolledEvent& event) {
            return OnGlobalMouseScrolled(event);
            });
    }

    void RecordingTab::OnRender()
    {
		ImGui::Begin(GetName().c_str());

        ImGui::BeginChild("RecordControls", ImVec2(300, 0), true);

        ImGui::Text("Name of Recording:");
        ImGui::InputText("##RecordingName", m_RecordingName, sizeof(m_RecordingName));

        if (m_ShowNameError)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text("Name cannot be blank!");
            ImGui::PopStyleColor();
        }

        ImGui::Spacing();
        ImGui::Checkbox("Record Mouse Movement", &m_RecordMouseMovement);

        ImGui::Spacing();
        ImGui::Text("Delay (seconds):");
        ImGui::SliderInt("##InitialDelay", &m_InitialDelay, 0, 10);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Check if GlobalInput is available
        if (!m_GlobalInputCapture)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                "Global input not available!");
        }
        else
        {
            ImGui::BeginDisabled(m_IsRecording || m_IsWaitingForDelay);
            if (ImGui::Button("Start Recording", ImVec2(-1, 40)))
            {
                StartRecording();
            }
            ImGui::EndDisabled();

            ImGui::BeginDisabled(!m_IsRecording && !m_IsWaitingForDelay);
            if (ImGui::Button("Stop Recording", ImVec2(-1, 40)))
            {
                StopRecording();
            }
            ImGui::EndDisabled();
        }

        ImGui::Spacing();

        if (m_IsWaitingForDelay)
        {
            ImGui::Text("Recording starts in %.1f seconds...", m_DelayTimer);
        }
        else if (m_IsRecording)
        {
            float duration = Application::GetTime() - m_RecordingStartTime;
            ImGui::Text("Recording... %.2fs", duration);
            ImGui::Text("Events recorded: %zu", m_CurrentRecording.Events.size());
        }

        ImGui::EndChild();

        ImGui::SameLine();

        // Right column - Event Panel
        ImGui::BeginChild("RecordEventPanel", ImVec2(0, 0), true);

        ImGui::Text("Recorded Events");
        ImGui::Separator();

        m_EventPanel.Render(ImVec2(0, 0));

        ImGui::EndChild();

		ImGui::End();
    }

    void RecordingTab::StartRecording()
    {
        m_ShowNameError = false;

        if (strlen(m_RecordingName) == 0)
        {
            m_ShowNameError = true;
            LUMINA_LOG_WARN("Recording name cannot be blank!");
            return;
        }

        if (!m_GlobalInputCapture)
        {
            LUMINA_LOG_ERROR("GlobalInput not available!");
            return;
        }

        // Clear previous recording
        m_EventPanel.Clear();
        m_CurrentRecording = Recording(m_RecordingName, m_RecordMouseMovement);

        // Enable event posting to application
        m_GlobalInputCapture->SetPostEventsToApplication(true);

        // Start global input capture
        if (!m_GlobalInputCapture->Start())
        {
            LUMINA_LOG_ERROR("Failed to start global input capture");
            return;
        }

        if (m_InitialDelay > 0)
        {
            m_IsWaitingForDelay = true;
            m_DelayTimer = static_cast<float>(m_InitialDelay);
            LUMINA_LOG_INFO("Starting recording with {} second delay...", m_InitialDelay);
        }
        else
        {
            m_IsRecording = true;
            m_RecordingStartTime = Application::GetTime();
            LUMINA_LOG_INFO("Recording started!");
        }
    }

    void RecordingTab::StopRecording()
    {
        if (m_IsWaitingForDelay)
        {
            m_IsWaitingForDelay = false;
            LUMINA_LOG_INFO("Recording cancelled");
            return;
        }

        m_IsRecording = false;
        m_CurrentRecording.TotalDuration = Application::GetTime() - m_RecordingStartTime;

        // Stop global input
        if (m_GlobalInputCapture)
        {
            m_GlobalInputCapture->Stop();
        }

        LUMINA_LOG_INFO("Recording stopped. Duration: {}s", m_CurrentRecording.TotalDuration);
        LUMINA_LOG_INFO("Total events: {}", m_CurrentRecording.Events.size());

        if (Serialization::SaveRecording(m_CurrentRecording))
        {
            LUMINA_LOG_INFO("Recording saved successfully!");
        }
        else
        {
            LUMINA_LOG_ERROR("Failed to save recording!");
        }
    }

    bool RecordingTab::OnGlobalKeyPressed(GlobalKeyPressedEvent& e)
    {
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
        m_EventPanel.AddEvent(event);

        return false;
    }

    bool RecordingTab::OnGlobalKeyReleased(GlobalKeyReleasedEvent& e)
    {
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
        m_EventPanel.AddEvent(event);

        return false;
    }

    bool RecordingTab::OnGlobalMouseButtonPressed(GlobalMouseButtonPressedEvent& e)
    {
        RecordedEvent event;
        event.Action = RecordedAction::MousePressed;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = e.GetX();
        event.MouseY = e.GetY();

        m_CurrentRecording.Events.push_back(event);
        m_EventPanel.AddEvent(event);

        return false;
    }

    bool RecordingTab::OnGlobalMouseButtonReleased(GlobalMouseButtonReleasedEvent& e)
    {
        RecordedEvent event;
        event.Action = RecordedAction::MouseReleased;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = e.GetX();
        event.MouseY = e.GetY();

        m_CurrentRecording.Events.push_back(event);
        m_EventPanel.AddEvent(event);

        return false;
    }

    bool RecordingTab::OnGlobalMouseMoved(GlobalMouseMovedEvent& e)
    {
        float currentTime = Application::GetTime();

        // Throttle mouse moves
        if (currentTime - m_LastMouseMoveTime < m_MouseMoveThreshold)
            return false;

        m_LastMouseMoveTime = currentTime;

        RecordedEvent event;
        event.Action = RecordedAction::MouseMoved;
        event.Time = currentTime - m_RecordingStartTime;
        event.MouseX = static_cast<float>(e.GetX());
        event.MouseY = static_cast<float>(e.GetY());

        m_CurrentRecording.Events.push_back(event);
        m_EventPanel.AddEvent(event);

        return false;
    }

    bool RecordingTab::OnGlobalMouseScrolled(GlobalMouseScrolledEvent& e)
    {
        RecordedEvent event;
        event.Action = RecordedAction::MouseScrolled;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.ScrollDX = e.GetDX();
        event.ScrollDY = e.GetDY();

        m_CurrentRecording.Events.push_back(event);
        m_EventPanel.AddEvent(event);

        return false;
    }
}
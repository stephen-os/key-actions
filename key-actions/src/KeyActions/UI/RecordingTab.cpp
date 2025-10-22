#include "RecordingTab.h"

#include "Lumina/Core/Log.h"

#include <cstring>

namespace KeyActions
{
    RecordingTab::RecordingTab() : Tab("Recording"), m_EventPanel(1000) {}
    
    void RecordingTab::OnAttach()
    {
        m_RecordingSession.SetEventRecordedCallback([this](const RecordedEvent& event) {
            m_EventPanel.AddEvent(event);
            });

        m_RecordingSession.SetRecordingStartedCallback([this]() {
            LUMINA_LOG_INFO("Recording started callback");
            });

        m_RecordingSession.SetRecordingStoppedCallback([this]() {
            const Recording& recording = m_RecordingSession.GetRecording();
            if (Serialization::SaveRecording(recording))
            {
                LUMINA_LOG_INFO("Recording saved successfully!");
            }
            else
            {
                LUMINA_LOG_ERROR("Failed to save recording!");
            }
            });

        LUMINA_LOG_INFO("Recording tab initialized");
    }

    void RecordingTab::OnDetach()
    {
        if (m_RecordingSession.IsRecording())
        {
            m_RecordingSession.Stop();
        }
    }

    void RecordingTab::OnUpdate(float timestep)
    {
        m_RecordingSession.Update(timestep);
    }

    void RecordingTab::OnEvent(Event& e)
    {
        Lumina::EventDispatcher dispatcher(e);

        // Forward  events to recording session
        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& event) {
            m_RecordingSession.OnKeyPressed(event);
            return false;
            });

        dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& event) {
            m_RecordingSession.OnKeyReleased(event);
            return false;
            });

        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& event) {
            m_RecordingSession.OnMouseButtonPressed(event);
            return false;
            });

        dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& event) {
            m_RecordingSession.OnMouseButtonReleased(event);
            return false;
            });

        dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& event) {
            m_RecordingSession.OnMouseMoved(event);
            return false;
            });

        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event) {
            m_RecordingSession.OnMouseScrolled(event);
            return false;
            });
    }

    void RecordingTab::OnRender()
    {
        // Left column - Controls
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

        // Recording controls
        bool isRecording = m_RecordingSession.IsRecording();
        bool isWaiting = m_RecordingSession.IsWaitingForDelay();

        ImGui::BeginDisabled(isRecording || isWaiting);
        if (ImGui::Button("Start Recording", ImVec2(-1, 40)))
        {
            StartRecording();
        }
        ImGui::EndDisabled();

        ImGui::BeginDisabled(!isRecording && !isWaiting);
        if (ImGui::Button("Stop Recording", ImVec2(-1, 40)))
        {
            StopRecording();
        }
        ImGui::EndDisabled();

        ImGui::Spacing();

        if (isWaiting)
        {
            ImGui::Text("Recording starts in %.1f seconds...", m_RecordingSession.GetDelayTimeRemaining());
        }
        else if (isRecording)
        {
            ImGui::Text("Recording... %.2fs", m_RecordingSession.GetElapsedTime());
            ImGui::Text("Events recorded: %zu", m_RecordingSession.GetEventCount());
        }

        ImGui::EndChild();

        ImGui::SameLine();

        // Right column - Event Panel
        ImGui::BeginChild("RecordEventPanel", ImVec2(0, 0), true);

        ImGui::Text("Recorded Events");
        ImGui::Separator();

        m_EventPanel.Render(ImVec2(0, 0));

        ImGui::EndChild();
    }

    void RecordingTab::StartRecording()
    {
        m_ShowNameError = false;

        if (strlen(m_RecordingName) == 0)
        {
            m_ShowNameError = true;
            return;
        }

        // Clear event panel
        m_EventPanel.Clear();

        // Create settings
        RecordingSettings settings;
        settings.Name = m_RecordingName;
        settings.RecordMouseMovement = m_RecordMouseMovement;
        settings.InitialDelaySeconds = m_InitialDelay;

        // Start recording
        m_RecordingSession.Start(settings);
    }

    void RecordingTab::StopRecording()
    {
        m_RecordingSession.Stop();
    }
}
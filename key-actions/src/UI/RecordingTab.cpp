#include "RecordingTab.h"

#include "Utils/Serialization.h"

#include "Lumina/Core/Application.h"
#include "Lumina/Core/Input.h"
#include "Lumina/Core/Log.h"
#include "Lumina/Events/KeyEvent.h"
#include "Lumina/Events/MouseEvent.h"
#include <cstring>

namespace Lumina
{
    RecordingTab::RecordingTab()
        : Tab("Recording")
    {
    }

    void RecordingTab::OnAttach()
    {
        AddConsoleMessage("Recording tab initialized.");
    }

    void RecordingTab::OnDetach()
    {
        if (m_IsRecording)
            StopRecording();
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
                AddConsoleMessage("Recording started!");
            }
        }
    }

    void RecordingTab::OnEvent(Event& e)
    {
        if (!m_IsRecording)
            return;

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& event) {
            return OnKeyPressed(event);
            });
        dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& event) {
            return OnKeyReleased(event);
            });
        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& event) {
            return OnMouseButtonPressed(event);
            });
        dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& event) {
            return OnMouseButtonReleased(event);
            });

        if (m_RecordMouseMovement)
        {
            dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& event) {
                return OnMouseMoved(event);
                });
        }

        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event) {
            return OnMouseScrolled(event);
            });
    }

    void RecordingTab::OnRender()
    {
        ImGui::Begin(GetName().c_str());
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

        // Right column - Console
        ImGui::BeginChild("RecordConsole", ImVec2(0, 0), true);

        ImGui::Text("Console");
        ImGui::Separator();

        ImGui::BeginChild("ConsoleMessages", ImVec2(0, -30), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (const auto& message : m_ConsoleMessages)
        {
            ImGui::TextUnformatted(message.c_str());
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();

        if (ImGui::Button("Clear Console", ImVec2(-1, 0)))
        {
            ClearConsole();
        }

        ImGui::EndChild();
        ImGui::End(); 
    }

    void RecordingTab::StartRecording()
    {
        m_ShowNameError = false;

        if (strlen(m_RecordingName) == 0)
        {
            m_ShowNameError = true;
            AddConsoleMessage("Error: Recording name cannot be blank!");
            return;
        }

        m_CurrentRecording = Recording(m_RecordingName, m_RecordMouseMovement);

        if (m_InitialDelay > 0)
        {
            m_IsWaitingForDelay = true;
            m_DelayTimer = static_cast<float>(m_InitialDelay);
            AddConsoleMessage("Starting recording with " + std::to_string(m_InitialDelay) + " second delay...");
        }
        else
        {
            m_IsRecording = true;
            m_RecordingStartTime = Application::GetTime();
            AddConsoleMessage("Recording started!");
        }
    }

    void RecordingTab::StopRecording()
    {
        if (m_IsWaitingForDelay)
        {
            m_IsWaitingForDelay = false;
            AddConsoleMessage("Recording cancelled.");
            return;
        }

        m_IsRecording = false;
        m_CurrentRecording.TotalDuration = Application::GetTime() - m_RecordingStartTime;

        AddConsoleMessage("Recording stopped. Duration: " + std::to_string(m_CurrentRecording.TotalDuration) + "s");
        AddConsoleMessage("Total events: " + std::to_string(m_CurrentRecording.Events.size()));

        if (Serialization::SaveRecording(m_CurrentRecording))
        {
            AddConsoleMessage("Recording saved successfully!");
        }
        else
        {
            AddConsoleMessage("Error: Failed to save recording!");
        }
    }

    void RecordingTab::ClearConsole()
    {
        m_ConsoleMessages.clear();
    }

    void RecordingTab::AddConsoleMessage(const std::string& message)
    {
        m_ConsoleMessages.push_back(message);

        if (m_ConsoleMessages.size() > m_MaxConsoleMessages)
        {
            m_ConsoleMessages.erase(m_ConsoleMessages.begin());
        }
    }

    bool RecordingTab::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.IsRepeat()) return false;

        RecordedEvent event;
        event.Action = ActionType::KeyPressed;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Key = e.GetKeyCode();

        m_CurrentRecording.Events.push_back(event);
        AddConsoleMessage(event.ToString());

        return false;
    }

    bool RecordingTab::OnKeyReleased(KeyReleasedEvent& e)
    {
        RecordedEvent event;
        event.Action = ActionType::KeyReleased;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Key = e.GetKeyCode();

        m_CurrentRecording.Events.push_back(event);
        AddConsoleMessage(event.ToString());

        return false;
    }

    bool RecordingTab::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        auto [x, y] = Input::GetMousePosition();

        RecordedEvent event;
        event.Action = ActionType::MousePressed;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = x;
        event.MouseY = y;

        m_CurrentRecording.Events.push_back(event);
        AddConsoleMessage(event.ToString());

        return false;
    }

    bool RecordingTab::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
    {
        auto [x, y] = Input::GetMousePosition();

        RecordedEvent event;
        event.Action = ActionType::MouseReleased;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = x;
        event.MouseY = y;

        m_CurrentRecording.Events.push_back(event);
        AddConsoleMessage(event.ToString());

        return false;
    }

    bool RecordingTab::OnMouseMoved(MouseMovedEvent& e)
    {
        float currentTime = Application::GetTime();

        if (currentTime - m_LastMouseMoveTime < m_MouseMoveThreshold)
            return false;

        m_LastMouseMoveTime = currentTime;

        RecordedEvent event;
        event.Action = ActionType::MouseMoved;
        event.Time = currentTime - m_RecordingStartTime;
        event.MouseX = e.GetX();
        event.MouseY = e.GetY();

        m_CurrentRecording.Events.push_back(event);
        AddConsoleMessage(event.ToString());

        return false;
    }

    bool RecordingTab::OnMouseScrolled(MouseScrolledEvent& e)
    {
        RecordedEvent event;
        event.Action = ActionType::MouseScrolled;
        event.Time = Application::GetTime() - m_RecordingStartTime;
        event.ScrollDX = static_cast<int>(e.GetXOffset());
        event.ScrollDY = static_cast<int>(e.GetYOffset());

        m_CurrentRecording.Events.push_back(event);
        AddConsoleMessage(event.ToString());

        return false;
    }
}
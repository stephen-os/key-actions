#include "InputCaptureTestLayer.h"
#include "Lumina/Core/Application.h"
#include "Lumina/Core/Log.h"

namespace Lumina
{
    InputCaptureTestLayer::InputCaptureTestLayer()
        : Layer("InputCaptureTest"), m_EventPanel(1000)
    {
    }

    InputCaptureTestLayer::~InputCaptureTestLayer()
    {
    }

    void InputCaptureTestLayer::OnAttach()
    {
        DockWindowSplit("Input Capture Test", DockPosition::Center);
        m_StartTime = Application::GetTime();

        // Create the input capture
        m_InputCapture = InputCapture::Create();

        if (!m_InputCapture)
        {
            LUMINA_LOG_ERROR("Failed to create InputCapture - platform not supported");
        }

        LUMINA_LOG_INFO("InputCaptureTestLayer attached");
    }

    void InputCaptureTestLayer::OnDetach()
    {
        if (m_InputCapture && m_InputCapture->IsActive())
        {
            m_InputCapture->Stop();
        }

        LUMINA_LOG_INFO("InputCaptureTestLayer detached");
    }

    void InputCaptureTestLayer::OnEvent(Event& e)
    {
        // We're using global hooks, so we don't need to handle Lumina events here
    }

    void InputCaptureTestLayer::OnUpdate(float timestep)
    {
    }

    void InputCaptureTestLayer::OnUIRender()
    {
        ImGui::Begin("Input Capture Test");

        ImGui::Text("Windows Input Capture Test");
        ImGui::Separator();

        if (!m_InputCapture)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "InputCapture not available on this platform");
            ImGui::End();
            return;
        }

        // Controls
        if (!m_InputCapture->IsActive())
        {
            if (ImGui::Button("Start Capturing", ImVec2(200, 40)))
            {
                // Set up callbacks
                m_InputCapture->SetKeyCallback([this](KeyCode key, bool pressed) {
                    OnGlobalKey(key, pressed);
                    });

                m_InputCapture->SetMouseButtonCallback([this](MouseCode button, bool pressed, int x, int y) {
                    OnGlobalMouseButton(button, pressed, x, y);
                    });

                m_InputCapture->SetMouseMoveCallback([this](int x, int y) {
                    if (m_RecordMouseMove)
                        OnGlobalMouseMove(x, y);
                    });

                m_InputCapture->SetMouseScrollCallback([this](int dx, int dy) {
                    OnGlobalMouseScroll(dx, dy);
                    });

                if (!m_InputCapture->Start())
                {
                    LUMINA_LOG_ERROR("Failed to start input capture");
                }
            }
        }
        else
        {
            if (ImGui::Button("Stop Capturing", ImVec2(200, 40)))
            {
                m_InputCapture->Stop();
            }
        }

        ImGui::SameLine();
        ImGui::Checkbox("Record Mouse Movement", &m_RecordMouseMove);

        // Stats
        ImGui::Separator();
        ImGui::Text("Statistics:");
        ImGui::Text("Key Presses: %d", m_KeyPressCount);
        ImGui::Text("Mouse Clicks: %d", m_MouseClickCount);
        ImGui::Text("Mouse Moves: %d", m_MouseMoveCount);
        ImGui::Text("Scrolls: %d", m_ScrollCount);
        ImGui::Text("Total Events: %zu", m_EventPanel.GetEventCount());

        ImGui::Separator();
        ImGui::Text("Status: %s", m_InputCapture->IsActive() ? "CAPTURING" : "STOPPED");

        if (m_InputCapture->IsActive())
        {
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f),
                "Capturing global input - press keys anywhere!");
        }

        ImGui::Separator();

        // Event panel
        m_EventPanel.Render(ImVec2(0, 0));

        ImGui::End();
    }

    void InputCaptureTestLayer::OnGlobalKey(KeyCode key, bool pressed)
    {
        RecordedEvent event;
        event.Action = pressed ? ActionType::KeyPressed : ActionType::KeyReleased;
        event.Time = Application::GetTime() - m_StartTime;
        event.Key = key;

        m_EventPanel.AddEvent(event);

        if (pressed)
            m_KeyPressCount++;
    }

    void InputCaptureTestLayer::OnGlobalMouseButton(MouseCode button, bool pressed, int x, int y)
    {
        RecordedEvent event;
        event.Action = pressed ? ActionType::MousePressed : ActionType::MouseReleased;
        event.Time = Application::GetTime() - m_StartTime;
        event.Button = button;
        event.MouseX = static_cast<float>(x);
        event.MouseY = static_cast<float>(y);

        m_EventPanel.AddEvent(event);

        if (pressed)
            m_MouseClickCount++;
    }

    void InputCaptureTestLayer::OnGlobalMouseMove(int x, int y)
    {
        float currentTime = Application::GetTime();

        // Throttle mouse moves
        if (currentTime - m_LastMouseMoveTime < m_MouseMoveThreshold)
            return;

        m_LastMouseMoveTime = currentTime;

        RecordedEvent event;
        event.Action = ActionType::MouseMoved;
        event.Time = currentTime - m_StartTime;
        event.MouseX = static_cast<float>(x);
        event.MouseY = static_cast<float>(y);

        m_EventPanel.AddEvent(event);
        m_MouseMoveCount++;
    }

    void InputCaptureTestLayer::OnGlobalMouseScroll(int dx, int dy)
    {
        RecordedEvent event;
        event.Action = ActionType::MouseScrolled;
        event.Time = Application::GetTime() - m_StartTime;
        event.ScrollDX = dx;
        event.ScrollDY = dy;

        m_EventPanel.AddEvent(event);
        m_ScrollCount++;
    }
}
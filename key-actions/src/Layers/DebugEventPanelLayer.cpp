#include "DebugEventPanelLayer.h"

#include "Lumina/Core/Application.h"
#include "Lumina/Core/Input.h"
#include "Lumina/Core/Log.h"
#include "Lumina/Events/KeyEvent.h"
#include "Lumina/Events/MouseEvent.h"

namespace Lumina
{
    DebugEventPanelLayer::DebugEventPanelLayer()
        : Layer("DebugEventPanel"), m_EventPanel(1000)
    {
    }

    void DebugEventPanelLayer::OnAttach()
    {
        DockWindowSplit("Debug Event Panel", DockPosition::Center);
        m_StartTime = Application::GetTime();

        LUMINA_LOG_INFO("DebugEventPanelLayer attached");

        // Add some test events
        AddTestEvents();
    }

    void DebugEventPanelLayer::OnDetach()
    {
        LUMINA_LOG_INFO("DebugEventPanelLayer detached");
    }

    void DebugEventPanelLayer::OnEvent(Event& e)
    {
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
        dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& event) {
            return OnMouseMoved(event);
            });
        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event) {
            return OnMouseScrolled(event);
            });
    }

    void DebugEventPanelLayer::OnUpdate(float timestep)
    {
    }

    void DebugEventPanelLayer::OnUIRender()
    {
        ImGui::Begin("Debug Event Panel");

        ImGui::Text("Event Panel Debugger");
        ImGui::Separator();

        if (ImGui::Button("Add Test Events"))
        {
            AddTestEvents();
        }

        ImGui::SameLine();

        if (ImGui::Button("Clear All"))
        {
            m_EventPanel.Clear();
        }

        ImGui::Separator();
        ImGui::Text("Total Events: %zu", m_EventPanel.GetEventCount());
        ImGui::Separator();

        m_EventPanel.Render(ImVec2(0, 0));

        ImGui::End();
    }

    void DebugEventPanelLayer::AddTestEvents()
    {
        float currentTime = Application::GetTime() - m_StartTime;

        // Add some sample events
        RecordedEvent event;

        // Key press
        event.Action = ActionType::KeyPressed;
        event.Time = currentTime;
        event.Key = KeyCode::A;
        m_EventPanel.AddEvent(event);

        // Key release
        event.Action = ActionType::KeyReleased;
        event.Time = currentTime + 0.1f;
        event.Key = KeyCode::A;
        m_EventPanel.AddEvent(event);

        // Mouse click
        event.Action = ActionType::MousePressed;
        event.Time = currentTime + 0.2f;
        event.Button = MouseCode::Left;
        event.MouseX = 150.0f;
        event.MouseY = 200.0f;
        m_EventPanel.AddEvent(event);

        event.Action = ActionType::MouseReleased;
        event.Time = currentTime + 0.3f;
        m_EventPanel.AddEvent(event);

        // Mouse move
        event.Action = ActionType::MouseMoved;
        event.Time = currentTime + 0.4f;
        event.MouseX = 300.0f;
        event.MouseY = 400.0f;
        m_EventPanel.AddEvent(event);

        // Mouse scroll
        event.Action = ActionType::MouseScrolled;
        event.Time = currentTime + 0.5f;
        event.ScrollDX = 0;
        event.ScrollDY = 1;
        m_EventPanel.AddEvent(event);

        LUMINA_LOG_INFO("Added test events");
    }

    bool DebugEventPanelLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.IsRepeat()) return false;

        RecordedEvent event;
        event.Action = ActionType::KeyPressed;
        event.Time = Application::GetTime() - m_StartTime;
        event.Key = e.GetKeyCode();

        m_EventPanel.AddEvent(event);

        return false;
    }

    bool DebugEventPanelLayer::OnKeyReleased(KeyReleasedEvent& e)
    {
        RecordedEvent event;
        event.Action = ActionType::KeyReleased;
        event.Time = Application::GetTime() - m_StartTime;
        event.Key = e.GetKeyCode();

        m_EventPanel.AddEvent(event);

        return false;
    }

    bool DebugEventPanelLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        auto [x, y] = Input::GetMousePosition();

        RecordedEvent event;
        event.Action = ActionType::MousePressed;
        event.Time = Application::GetTime() - m_StartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = x;
        event.MouseY = y;

        m_EventPanel.AddEvent(event);

        return false;
    }

    bool DebugEventPanelLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
    {
        auto [x, y] = Input::GetMousePosition();

        RecordedEvent event;
        event.Action = ActionType::MouseReleased;
        event.Time = Application::GetTime() - m_StartTime;
        event.Button = e.GetMouseButton();
        event.MouseX = x;
        event.MouseY = y;

        m_EventPanel.AddEvent(event);

        return false;
    }

    bool DebugEventPanelLayer::OnMouseMoved(MouseMovedEvent& e)
    {
        float currentTime = Application::GetTime();

        // Throttle mouse move events
        if (currentTime - m_LastMouseMoveTime < m_MouseMoveThreshold)
            return false;

        m_LastMouseMoveTime = currentTime;

        RecordedEvent event;
        event.Action = ActionType::MouseMoved;
        event.Time = currentTime - m_StartTime;
        event.MouseX = e.GetX();
        event.MouseY = e.GetY();

        m_EventPanel.AddEvent(event);

        return false;
    }

    bool DebugEventPanelLayer::OnMouseScrolled(MouseScrolledEvent& e)
    {
        RecordedEvent event;
        event.Action = ActionType::MouseScrolled;
        event.Time = Application::GetTime() - m_StartTime;
        event.ScrollDX = static_cast<int>(e.GetXOffset());
        event.ScrollDY = static_cast<int>(e.GetYOffset());

        m_EventPanel.AddEvent(event);

        return false;
    }
}
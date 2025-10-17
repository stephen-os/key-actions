#pragma once

#include "Lumina/Core/Layer.h"
#include "UI/EventPanel.h"
#include "Core/Recording.h"

#include "Lumina/Events/KeyEvent.h"
#include "Lumina/Events/MouseEvent.h"

namespace Lumina
{
    class DebugEventPanelLayer : public Layer
    {
    public:
        DebugEventPanelLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;
        virtual void OnUpdate(float timestep) override;
        virtual void OnUIRender() override;

    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
        bool OnMouseMoved(MouseMovedEvent& e);
        bool OnMouseScrolled(MouseScrolledEvent& e);

        void AddTestEvents();

    private:
        EventPanel m_EventPanel;
        float m_StartTime;
        float m_LastMouseMoveTime = 0.0f;
        float m_MouseMoveThreshold = 0.02f;
    };
}
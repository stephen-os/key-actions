#pragma once
#include "Lumina/Core/Layer.h"
#include "UI/EventPanel.h"
#include "Core/Recording.h"
#include "Input/InputCapture.h"
#include <memory>

namespace Lumina
{
    class InputCaptureTestLayer : public Layer
    {
    public:
        InputCaptureTestLayer();
        ~InputCaptureTestLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;
        virtual void OnUpdate(float timestep) override;
        virtual void OnUIRender() override;

    private:
        void OnGlobalKey(KeyCode key, bool pressed);
        void OnGlobalMouseButton(MouseCode button, bool pressed, int x, int y);
        void OnGlobalMouseMove(int x, int y);
        void OnGlobalMouseScroll(int dx, int dy);

    private:
        std::unique_ptr<InputCapture> m_InputCapture;
        EventPanel m_EventPanel;
        float m_StartTime;
        bool m_RecordMouseMove = false;
        float m_LastMouseMoveTime = 0.0f;
        float m_MouseMoveThreshold = 0.02f;

        // Stats
        int m_KeyPressCount = 0;
        int m_MouseClickCount = 0;
        int m_MouseMoveCount = 0;
        int m_ScrollCount = 0;
    };
}
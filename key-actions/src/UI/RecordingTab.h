// UI/RecordingTab.h
#pragma once
#include "Tab.h"

#include "Core/Recording.h"

#include <vector>
#include <string>

namespace Lumina
{
    class KeyPressedEvent;
    class KeyReleasedEvent;
    class MouseButtonPressedEvent;
    class MouseButtonReleasedEvent;
    class MouseMovedEvent;
    class MouseScrolledEvent;

    class RecordingTab : public Tab
    {
    public:
        RecordingTab();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(float timestep) override;
        virtual void OnEvent(Event& e) override;
        virtual void OnRender() override;

    private:
        void StartRecording();
        void StopRecording();
        void ClearConsole();
        void AddConsoleMessage(const std::string& message);

        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
        bool OnMouseMoved(MouseMovedEvent& e);
        bool OnMouseScrolled(MouseScrolledEvent& e);

    private:
        // UI State
        char m_RecordingName[256] = "";
        bool m_RecordMouseMovement = false;
        int m_InitialDelay = 0;
        bool m_ShowNameError = false;

        // Recording State
        bool m_IsRecording = false;
        float m_RecordingStartTime = 0.0f;
        float m_DelayTimer = 0.0f;
        bool m_IsWaitingForDelay = false;
        Recording m_CurrentRecording;

        // Console
        std::vector<std::string> m_ConsoleMessages;
        int m_MaxConsoleMessages = 1000;

        // Mouse tracking
        float m_LastMouseMoveTime = 0.0f;
        float m_MouseMoveThreshold = 0.02f;
    };
}
#pragma once
#include "Tab.h"
#include "EventPanel.h"
#include "Core/Recording.h"
#include "Lumina/Input/GlobalInputCapture.h"
#include <vector>
#include <string>
#include <memory>

namespace Lumina
{
    class GlobalKeyPressedEvent;
    class GlobalKeyReleasedEvent;
    class GlobalMouseButtonPressedEvent;
    class GlobalMouseButtonReleasedEvent;
    class GlobalMouseMovedEvent;
    class GlobalMouseScrolledEvent;

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

        bool OnGlobalKeyPressed(GlobalKeyPressedEvent& e);
        bool OnGlobalKeyReleased(GlobalKeyReleasedEvent& e);
        bool OnGlobalMouseButtonPressed(GlobalMouseButtonPressedEvent& e);
        bool OnGlobalMouseButtonReleased(GlobalMouseButtonReleasedEvent& e);
        bool OnGlobalMouseMoved(GlobalMouseMovedEvent& e);
        bool OnGlobalMouseScrolled(GlobalMouseScrolledEvent& e);

    private:
        // Global input system
        std::unique_ptr<GlobalInputCapture> m_GlobalInputCapture;

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

        // Event panel for displaying recorded events
        EventPanel m_EventPanel;

        // Mouse tracking
        float m_LastMouseMoveTime = 0.0f;
        float m_MouseMoveThreshold = 0.02f;
    };
}
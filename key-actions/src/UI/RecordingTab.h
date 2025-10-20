#pragma once
#include "Tab.h"
#include "EventPanel.h"
#include "Core/RecordingSession.h"
#include "Core/Serialization.h"

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

    private:
        RecordingSession m_RecordingSession;

        // UI State
        char m_RecordingName[256] = "";
        bool m_RecordMouseMovement = false;
        int m_InitialDelay = 0;
        bool m_ShowNameError = false;

        // Event panel for displaying recorded events
        EventPanel m_EventPanel;
    };
}
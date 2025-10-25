#pragma once
#include "Tab.h"

#include "EventPanel.h"

#include "KeyActions/Core/RecordingSession.h"
#include "KeyActions/Core/Serialization.h"

#include "Lumina/Events/GlobalKeyEvent.h"
#include "Lumina/Events/GlobalMouseEvent.h"

#include "Lumina/Core/Input.h"

namespace KeyActions
{
    class RecordingTab : public Tab
    {
    public:
        using KeyCombo = Lumina::KeyCombo;

        using KeyPressedEvent = Lumina::GlobalKeyPressedEvent;
		using KeyReleasedEvent = Lumina::GlobalKeyReleasedEvent;

		using MouseButtonPressedEvent = Lumina::GlobalMouseButtonPressedEvent;
		using MouseButtonReleasedEvent = Lumina::GlobalMouseButtonReleasedEvent;
		using MouseMovedEvent = Lumina::GlobalMouseMovedEvent;
		using MouseScrolledEvent = Lumina::GlobalMouseScrolledEvent;

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

        KeyCombo m_CapturedKeyCombo;

        // Event panel for displaying recorded events
        EventPanel m_EventPanel;
    };
}
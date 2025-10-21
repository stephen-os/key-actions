#pragma once
#include "Tab.h"
#include "Core/Settings.h"
#include "Lumina/Core/Input.h"
#include <string>
#include <vector>

namespace Lumina
{
    class SettingsTab : public Tab
    {
    public:
        SettingsTab();
        virtual ~SettingsTab() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float timestep) override;
        void OnEvent(Event& e) override;
        void OnRender() override;

    private:
        int m_AutoSaveIntervalBuffer;
        bool m_AutoSaveEnabledBuffer;

        bool m_CapturingStartRecording = false;
        bool m_CapturingStopRecording = false;
        bool m_CapturingPlayRecording = false;
        bool m_CapturingStopPlayback = false;

        std::vector<KeyCode> m_CapturedKeys;
        std::string m_ValidationError;
    };
}
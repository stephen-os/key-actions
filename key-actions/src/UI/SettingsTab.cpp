// UI/SettingsTab.cpp
#include "SettingsTab.h"
#include "Lumina/Core/Log.h"

namespace Lumina
{
    SettingsTab::SettingsTab()
        : Tab("Settings")
    {
    }

    void SettingsTab::OnAttach()
    {
        LUMINA_LOG_INFO("Settings tab initialized");
    }

    void SettingsTab::OnDetach()
    {
    }

    void SettingsTab::OnUpdate(float timestep)
    {
    }

    void SettingsTab::OnEvent(Event& e)
    {
    }

    void SettingsTab::OnRender()
    {
    }
}
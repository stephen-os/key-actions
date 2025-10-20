#include "KeyActionsLayer.h"

#include "UI/RecordingTab.h"
#include "UI/PlaybackTab.h"
#include "UI/SettingsTab.h"

#include "Lumina/Core/Log.h"

namespace Lumina
{
    KeyActionsLayer::KeyActionsLayer() : Layer("KeyActions") {}

    void KeyActionsLayer::OnAttach()
    {
		auto recordingTab = std::make_shared<RecordingTab>();
        auto settingsTab = std::make_shared<SettingsTab>();
		auto playbackTab = std::make_shared<PlaybackTab>();

        m_Tabs.push_back(recordingTab);
		m_Tabs.push_back(settingsTab);
        m_Tabs.push_back(playbackTab);

        // Initialize all tabs
        for (auto& tab : m_Tabs)
        {
            tab->OnAttach();
        }

        DockWindowSplit(recordingTab->GetName(), DockPosition::Center);

		DockWindowTabbed(settingsTab->GetName(), recordingTab->GetName());
		DockWindowTabbed(playbackTab->GetName(), recordingTab->GetName());

        LUMINA_LOG_INFO("KeyActions layer attached with {} tabs", m_Tabs.size());
    }

    void KeyActionsLayer::OnDetach()
    {
        for (auto& tab : m_Tabs)
        {
            tab->OnDetach();
        }

        LUMINA_LOG_INFO("KeyActions layer detached");
    }

    void KeyActionsLayer::OnEvent(Event& e)
    {
        // Forward events to all tabs
        for (auto& tab : m_Tabs)
        {
            tab->OnEvent(e);
        }
    }

    void KeyActionsLayer::OnUpdate(float timestep)
    {
        // Update all tabs
        for (auto& tab : m_Tabs)
        {
            tab->OnUpdate(timestep);
        }
    }

    void KeyActionsLayer::OnUIRender()
    {
        for (auto& tab : m_Tabs)
        {
            tab->OnRender();
        }
    }
} 
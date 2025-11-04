#include "KeyActionsLayer.h"

#include "KeyActions/UI/RecordingTab.h"
#include "KeyActions/UI/PlaybackTab.h"
#include "KeyActions/UI/SettingsTab.h"

// Testing
#include "KeyActions/UI/NodeEditorTab.h"

#include "KeyActions/Core/Settings.h"

#include "Lumina/Core/Log.h"
#include "Lumina/Core/Assert.h"

#include "KeyActions/UI/Styles/Theme.h"

namespace KeyActions
{
    KeyActionsLayer::KeyActionsLayer() : Layer("KeyActions") {}

    void KeyActionsLayer::OnAttach()
    {
        m_GlobalInputCapture = Lumina::GlobalInputCapture::Create();
        LUMINA_ASSERT(m_GlobalInputCapture, "Failed to create global input capture");

        m_GlobalInputCapture->SetPostEventsToApplication(true);
        m_GlobalInputCapture->Start();

        Settings::Init();

        UI::ApplyTheme(); 

        auto recordingTab = std::make_shared<RecordingTab>();
        auto playbackTab = std::make_shared<PlaybackTab>();
        auto settingsTab = std::make_shared<SettingsTab>();
		auto nodeEditorTab = std::make_shared<NodeEditorTab>();

        m_Tabs.push_back(recordingTab);
        m_Tabs.push_back(playbackTab);
        m_Tabs.push_back(settingsTab);
		m_Tabs.push_back(nodeEditorTab); // Testing

        for (auto& tab : m_Tabs)
        {
            tab->OnAttach();
        }
        
        recordingTab->Show();
        playbackTab->Hide();
        settingsTab->Hide();

        LUMINA_LOG_INFO("KeyActions layer attached with {} tabs", m_Tabs.size());
    }

    void KeyActionsLayer::OnDetach()
    {
        for (auto& tab : m_Tabs)
        {
            tab->OnDetach();
        }

        Settings::Shutdown();

        m_GlobalInputCapture->Stop();
	
        LUMINA_LOG_INFO("KeyActions layer detached");
    }

    void KeyActionsLayer::OnEvent(Event& e)
    {
        for (auto& tab : m_Tabs)
        {
            if (tab->IsVisible())
            {
                tab->OnEvent(e);
            }
        }
    }

    void KeyActionsLayer::OnUpdate(float timestep)
    {
        for (auto& tab : m_Tabs)
        {
            if (tab->IsVisible())
            {
                tab->OnUpdate(timestep);
            }
        }
    }

    void KeyActionsLayer::OnUIRender()
    {
        if (ImGui::BeginMainMenuBar())
        {
            for (size_t i = 0; i < m_Tabs.size(); i++)
            {
                bool isActive = (m_ActiveTabIndex == i);

                if (isActive)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
                }

                if (ImGui::Button(m_Tabs[i]->GetName().c_str()))
                {
                    m_ActiveTabIndex = i;
                    for (size_t j = 0; j < m_Tabs.size(); j++)
                    {
                        if (j == m_ActiveTabIndex)
                            m_Tabs[j]->SetVisible(true);
                        else
                            m_Tabs[j]->SetVisible(false);
					}   
                }

                if (isActive)
                {
                    ImGui::PopStyleColor();
                }
            }
            ImGui::EndMainMenuBar();
        }

        for (auto& tab : m_Tabs)
        {
            if (tab->IsVisible())
            {
                tab->OnRender();
            }
        }
    }
} 
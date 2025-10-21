#include "SettingsTab.h"

#include "Lumina/Core/Log.h"

#include "Lumina/Events/WindowKeyEvent.h"

#include <cstring>

namespace Lumina
{
    SettingsTab::SettingsTab() : Tab("Settings") {}

    void SettingsTab::OnAttach()
    {
        const auto& data = Settings::Data();

        std::string folderPath = data.RecordingsFolder.string();
        strncpy(m_RecordingsFolderBuffer, folderPath.c_str(), sizeof(m_RecordingsFolderBuffer) - 1);
        m_RecordingsFolderBuffer[sizeof(m_RecordingsFolderBuffer) - 1] = '\0';

        m_AutoSaveIntervalBuffer = data.AutoSaveIntervalSeconds;
        m_AutoSaveEnabledBuffer = data.AutoSaveEnabled;

        LUMINA_LOG_INFO("Settings tab initialized");
    }

    void SettingsTab::OnDetach()
    {
        if (Settings::IsModified())
        {
            LUMINA_LOG_WARN("Settings tab closed with unsaved changes");
        }
    }

    void SettingsTab::OnUpdate(float timestep)
    {
    }

    void SettingsTab::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowKeyPressedEvent>([this](WindowKeyPressedEvent& event) {
            if (event.IsRepeat())
                return false;

            KeyCode keyCode = event.GetKeyCode();

            if (m_CapturingStartRecording || m_CapturingStopRecording ||
                m_CapturingPlayRecording || m_CapturingStopPlayback)
            {
                if (keyCode == KeyCode::Escape)
                {
                    m_CapturingStartRecording = false;
                    m_CapturingStopRecording = false;
                    m_CapturingPlayRecording = false;
                    m_CapturingStopPlayback = false;
                    m_CapturedKeys.clear();
                    return true;
                }

                if (std::find(m_CapturedKeys.begin(), m_CapturedKeys.end(), keyCode) == m_CapturedKeys.end())
                {
                    m_CapturedKeys.push_back(keyCode);
                }
                return true;
            }

            return false;
            });

        dispatcher.Dispatch<WindowKeyReleasedEvent>([this](WindowKeyReleasedEvent& event) {
            if (m_CapturingStartRecording || m_CapturingStopRecording ||
                m_CapturingPlayRecording || m_CapturingStopPlayback)
            {
                if (!m_CapturedKeys.empty())
                {
                    auto& settings = Settings::GetMutable();
                    KeyCombo newCombo;
                    newCombo.Keys = m_CapturedKeys;

                    if (m_CapturingStartRecording)
                    {
                        settings.StartRecording = newCombo;
                        m_CapturingStartRecording = false;
                    }
                    else if (m_CapturingStopRecording)
                    {
                        settings.StopRecording = newCombo;
                        m_CapturingStopRecording = false;
                    }
                    else if (m_CapturingPlayRecording)
                    {
                        settings.PlayRecording = newCombo;
                        m_CapturingPlayRecording = false;
                    }
                    else if (m_CapturingStopPlayback)
                    {
                        settings.StopPlayback = newCombo;
                        m_CapturingStopPlayback = false;
                    }

                    
                    m_CapturedKeys.clear();
                }
                return true;
            }

            return false;
            });
    }

    void SettingsTab::OnRender()
    {
        ImGui::BeginChild("SettingsPanel", ImVec2(0, 0), true);

        ImGui::Text("Settings");
        ImGui::Separator();
        ImGui::Spacing();

        auto& settings = Settings::GetMutable();

        ImGui::Text("Recording Keybinds");
        ImGui::Spacing();

        std::string startRecText = m_CapturingStartRecording ? "Press keys... (ESC to cancel)" : "Start Recording";
        if (ImGui::Button(startRecText.c_str(), ImVec2(250, 0)))
        {
            m_CapturingStartRecording = true;
            m_CapturedKeys.clear();
        }
        ImGui::SameLine();
        if (m_CapturingStartRecording && !m_CapturedKeys.empty())
        {
            std::string keysText;
            for (size_t i = 0; i < m_CapturedKeys.size(); i++)
            {
                keysText += Input::KeyCodeToString(m_CapturedKeys[i]);
                if (i < m_CapturedKeys.size() - 1)
                    keysText += " + ";
            }
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text(": %s", keysText.c_str());
            ImGui::PopStyleColor();
        }
        else if (!settings.StartRecording.Keys.empty())
        {
            std::string keysText;
            for (size_t i = 0; i < settings.StartRecording.Keys.size(); i++)
            {
                keysText += Input::KeyCodeToString(settings.StartRecording.Keys[i]);
                if (i < settings.StartRecording.Keys.size() - 1)
                    keysText += " + ";
            }
            ImGui::Text(": %s", keysText.c_str());
        }

        std::string stopRecText = m_CapturingStopRecording ? "Press keys... (ESC to cancel)" : "Stop Recording";
        if (ImGui::Button(stopRecText.c_str(), ImVec2(250, 0)))
        {
            m_CapturingStopRecording = true;
            m_CapturedKeys.clear();
        }
        ImGui::SameLine();
        if (m_CapturingStopRecording && !m_CapturedKeys.empty())
        {
            std::string keysText;
            for (size_t i = 0; i < m_CapturedKeys.size(); i++)
            {
                keysText += Input::KeyCodeToString(m_CapturedKeys[i]);
                if (i < m_CapturedKeys.size() - 1)
                    keysText += " + ";
            }
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text(": %s", keysText.c_str());
            ImGui::PopStyleColor();
        }
        else if (!settings.StopRecording.Keys.empty())
        {
            std::string keysText;
            for (size_t i = 0; i < settings.StopRecording.Keys.size(); i++)
            {
                keysText += Input::KeyCodeToString(settings.StopRecording.Keys[i]);
                if (i < settings.StopRecording.Keys.size() - 1)
                    keysText += " + ";
            }
            ImGui::Text(": %s", keysText.c_str());
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Playback Keybinds");
        ImGui::Spacing();

        std::string playRecText = m_CapturingPlayRecording ? "Press keys... (ESC to cancel)" : "Play Recording";
        if (ImGui::Button(playRecText.c_str(), ImVec2(250, 0)))
        {
            m_CapturingPlayRecording = true;
            m_CapturedKeys.clear();
        }
        ImGui::SameLine();
        if (m_CapturingPlayRecording && !m_CapturedKeys.empty())
        {
            std::string keysText;
            for (size_t i = 0; i < m_CapturedKeys.size(); i++)
            {
                keysText += Input::KeyCodeToString(m_CapturedKeys[i]);
                if (i < m_CapturedKeys.size() - 1)
                    keysText += " + ";
            }
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text(": %s", keysText.c_str());
            ImGui::PopStyleColor();
        }
        else if (!settings.PlayRecording.Keys.empty())
        {
            std::string keysText;
            for (size_t i = 0; i < settings.PlayRecording.Keys.size(); i++)
            {
                keysText += Input::KeyCodeToString(settings.PlayRecording.Keys[i]);
                if (i < settings.PlayRecording.Keys.size() - 1)
                    keysText += " + ";
            }
            ImGui::Text(": %s", keysText.c_str());
        }

        std::string stopPlayText = m_CapturingStopPlayback ? "Press keys... (ESC to cancel)" : "Stop Playback";
        if (ImGui::Button(stopPlayText.c_str(), ImVec2(250, 0)))
        {
            m_CapturingStopPlayback = true;
            m_CapturedKeys.clear();
        }
        ImGui::SameLine();
        if (m_CapturingStopPlayback && !m_CapturedKeys.empty())
        {
            std::string keysText;
            for (size_t i = 0; i < m_CapturedKeys.size(); i++)
            {
                keysText += Input::KeyCodeToString(m_CapturedKeys[i]);
                if (i < m_CapturedKeys.size() - 1)
                    keysText += " + ";
            }
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text(": %s", keysText.c_str());
            ImGui::PopStyleColor();
        }
        else if (!settings.StopPlayback.Keys.empty())
        {
            std::string keysText;
            for (size_t i = 0; i < settings.StopPlayback.Keys.size(); i++)
            {
                keysText += Input::KeyCodeToString(settings.StopPlayback.Keys[i]);
                if (i < settings.StopPlayback.Keys.size() - 1)
                    keysText += " + ";
            }
            ImGui::Text(": %s", keysText.c_str());
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Application Settings");
        ImGui::Spacing();

        ImGui::Text("Recordings Folder:");
        if (ImGui::InputText("##RecordingsFolder", m_RecordingsFolderBuffer, sizeof(m_RecordingsFolderBuffer)))
        {
            settings.RecordingsFolder = m_RecordingsFolderBuffer;
        }

        ValidationResult folderValidation = Settings::ValidateRecordingsFolder(settings.RecordingsFolder);
        if (!folderValidation.IsValid)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text("%s", folderValidation.ErrorMessage.c_str());
            ImGui::PopStyleColor();

            ImGui::SameLine();
            if (ImGui::Button("Revert##Folder"))
            {
                Settings::RevertRecordingsFolder();
                std::string folderPath = settings.RecordingsFolder.string();
                strncpy(m_RecordingsFolderBuffer, folderPath.c_str(), sizeof(m_RecordingsFolderBuffer) - 1);
                m_RecordingsFolderBuffer[sizeof(m_RecordingsFolderBuffer) - 1] = '\0';
            }
        }

        ImGui::Spacing();

        if (ImGui::Checkbox("Auto Save Enabled", &m_AutoSaveEnabledBuffer))
        {
            settings.AutoSaveEnabled = m_AutoSaveEnabledBuffer;
        }

        ImGui::Text("Auto Save Interval (seconds):");
        if (ImGui::SliderInt("##AutoSaveInterval", &m_AutoSaveIntervalBuffer, 10, 3600))
        {
            settings.AutoSaveIntervalSeconds = m_AutoSaveIntervalBuffer;
        }

        ValidationResult intervalValidation = Settings::ValidateAutoSaveInterval(settings.AutoSaveIntervalSeconds);
        if (!intervalValidation.IsValid)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text("%s", intervalValidation.ErrorMessage.c_str());
            ImGui::PopStyleColor();

            ImGui::SameLine();
            if (ImGui::Button("Revert##Interval"))
            {
                Settings::RevertAutoSaveInterval();
                m_AutoSaveIntervalBuffer = settings.AutoSaveIntervalSeconds;
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        bool hasChanges = Settings::IsModified();
        bool isValid = Settings::IsValid();

        if (hasChanges)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
            ImGui::Text("You have unsaved changes");
            ImGui::PopStyleColor();
        }

        ImGui::BeginDisabled(!hasChanges || !isValid);
        if (ImGui::Button("Save Settings", ImVec2(150, 40)))
        {
            if (Settings::Save())
            {
                LUMINA_LOG_INFO("Settings saved successfully");
                m_ValidationError.clear();
            }
            else
            {
                LUMINA_LOG_ERROR("Failed to save settings");
                m_ValidationError = "Failed to save settings";
            }
        }
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(!hasChanges);
        if (ImGui::Button("Revert All", ImVec2(150, 40)))
        {
            Settings::RevertAll();

            const auto& data = Settings::Data();
            std::string folderPath = data.RecordingsFolder.string();
            strncpy(m_RecordingsFolderBuffer, folderPath.c_str(), sizeof(m_RecordingsFolderBuffer) - 1);
            m_RecordingsFolderBuffer[sizeof(m_RecordingsFolderBuffer) - 1] = '\0';
            m_AutoSaveIntervalBuffer = data.AutoSaveIntervalSeconds;
            m_AutoSaveEnabledBuffer = data.AutoSaveEnabled;

            LUMINA_LOG_INFO("Settings reverted to last saved state");
        }
        ImGui::EndDisabled();

        if (!m_ValidationError.empty())
        {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text("%s", m_ValidationError.c_str());
            ImGui::PopStyleColor();
        }

        ImGui::EndChild();
    }
}
#include "SettingsTab.h"

#include "Lumina/Core/Log.h"
#include "Lumina/Events/WindowKeyEvent.h"

#include "KeyActions/UI/Styles/Theme.h"

#include "KeyActions/UI/Components/Buttons.h"
#include "KeyActions/UI/Components/Text.h"
#include "KeyActions/UI/Components/Headings.h"
#include "KeyActions/UI/Components/Layouts.h"
#include "KeyActions/UI/Components/Keybinds.h"
#include "KeyActions/UI/Components/Inputs.h"

#include "Components/FileDialog.h"

#include <ImGuiFileDialog.h>

namespace KeyActions
{
    SettingsTab::SettingsTab() : Tab("Settings") {}

    void SettingsTab::OnAttach()
    {
        const auto& data = Settings::Data();
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
        Lumina::EventDispatcher dispatcher(e);

        dispatcher.Dispatch<Lumina::WindowKeyPressedEvent>([this](Lumina::WindowKeyPressedEvent& event) {
            if (event.IsRepeat())
                return false;

            Lumina::KeyCode keyCode = event.GetKeyCode();

            if (m_CapturingStartRecording || m_CapturingStopRecording ||
                m_CapturingPlayRecording || m_CapturingStopPlayback)
            {
                if (keyCode == Lumina::KeyCode::Escape)
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

        dispatcher.Dispatch<Lumina::WindowKeyReleasedEvent>([this](Lumina::WindowKeyReleasedEvent& event) {
            if (m_CapturingStartRecording || m_CapturingStopRecording ||
                m_CapturingPlayRecording || m_CapturingStopPlayback)
            {
                if (!m_CapturedKeys.empty())
                {
                    auto& settings = Settings::GetMutable();
                    Lumina::KeyCombo newCombo;
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
        UI::BeginPanel("SettingsPanel", ImVec2(0, 0), true);

        UI::HeadingOne("Settings");

        auto& settings = Settings::GetMutable();

        UI::HeadingTwo("Recording Keybinds");

        UI::Spacing();
\
        if (UI::ButtonKeybind("Start Recording", m_CapturingStartRecording, m_CapturedKeys))
        {
            m_CapturingStartRecording = true;
            m_CapturingStopRecording = false;
            m_CapturingPlayRecording = false;
            m_CapturingStopPlayback = false;
            m_CapturedKeys.clear();
        }
        if (m_CapturingStartRecording && !m_CapturedKeys.empty())
        {
            UI::DisplayKeybindCapturing(m_CapturedKeys);
        }
        else if (!settings.StartRecording.Keys.empty())
        {
            UI::DisplayKeybind(settings.StartRecording.Keys);
        }

        UI::Spacing();

        if (UI::ButtonKeybind("Stop Recording", m_CapturingStopRecording, m_CapturedKeys))
        {
            m_CapturingStartRecording = false;
            m_CapturingStopRecording = true;
            m_CapturingPlayRecording = false;
            m_CapturingStopPlayback = false;
            m_CapturedKeys.clear();
        }
        if (m_CapturingStopRecording && !m_CapturedKeys.empty())
        {
            UI::DisplayKeybindCapturing(m_CapturedKeys);
        }
        else if (!settings.StopRecording.Keys.empty())
        {
            UI::DisplayKeybind(settings.StopRecording.Keys);
        }

        UI::SectionSeparator();

        UI::HeadingTwo("Playback Keybinds");

        UI::Spacing();

        if (UI::ButtonKeybind("Play Recording", m_CapturingPlayRecording, m_CapturedKeys))
        {
            m_CapturingStartRecording = false;
            m_CapturingStopRecording = false;
            m_CapturingPlayRecording = true;
            m_CapturingStopPlayback = false;
            m_CapturedKeys.clear();
        }
        if (m_CapturingPlayRecording && !m_CapturedKeys.empty())
        {
            UI::DisplayKeybindCapturing(m_CapturedKeys);
        }
        else if (!settings.PlayRecording.Keys.empty())
        {
            UI::DisplayKeybind(settings.PlayRecording.Keys);
        }

        UI::Spacing();

        if (UI::ButtonKeybind("Stop Playback", m_CapturingStopPlayback, m_CapturedKeys))
        {
            m_CapturingStartRecording = false;
            m_CapturingStopRecording = false;
            m_CapturingPlayRecording = false;
            m_CapturingStopPlayback = true;
            m_CapturedKeys.clear();
        }
        if (m_CapturingStopPlayback && !m_CapturedKeys.empty())
        {
            UI::DisplayKeybindCapturing(m_CapturedKeys);
        }
        else if (!settings.StopPlayback.Keys.empty())
        {
            UI::DisplayKeybind(settings.StopPlayback.Keys);
        }

        UI::SectionSeparator();

        UI::HeadingTwo("Application Settings");

        UI::Spacing();

        UI::Label("Recordings Folder:");
        
        UI::TextHighlight("%s", settings.RecordingsFolder.string().c_str());

        std::string folderPathStr = settings.RecordingsFolder.string();
        if (UI::FileDialogFolder("ChooseRecordingsFolder", "Choose Recordings Folder", folderPathStr))
        {
            settings.RecordingsFolder = folderPathStr;
        }

        if (!std::filesystem::exists(settings.RecordingsFolder))
        {
            UI::Spacing();
            UI::TextWarning("Warning: Directory does not exist. It will be created when saving.");
        }

        UI::Spacing();

        if (UI::CheckboxStyled("Auto Save Enabled", &m_AutoSaveEnabledBuffer))
        {
            settings.AutoSaveEnabled = m_AutoSaveEnabledBuffer;
        }

        UI::Spacing();

        UI::Label("Auto Save Interval (seconds):");
        if (UI::SliderIntStyled("##AutoSaveInterval", &m_AutoSaveIntervalBuffer, 10, 3600))
        {
            settings.AutoSaveIntervalSeconds = m_AutoSaveIntervalBuffer;
        }

        UI::SectionSeparator();

        bool hasChanges = Settings::IsModified();

        if (hasChanges)
        {
            UI::TextWarning("You have unsaved changes");
            UI::Spacing();
        }

        if (UI::ButtonSuccess("Save Settings", UI::Sizes::ButtonWide, hasChanges))
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

        UI::SameLine();

        if (UI::ButtonDanger("Revert All", UI::Sizes::ButtonWide, hasChanges))
        {
            Settings::Load();

            const auto& data = Settings::Data();
            m_AutoSaveIntervalBuffer = data.AutoSaveIntervalSeconds;
            m_AutoSaveEnabledBuffer = data.AutoSaveEnabled;

            LUMINA_LOG_INFO("Settings reverted to last saved state");
        }

        if (!m_ValidationError.empty())
        {
            UI::Spacing();
            UI::TextDanger("%s", m_ValidationError.c_str());
        }

        UI::EndPanel();
    }
}
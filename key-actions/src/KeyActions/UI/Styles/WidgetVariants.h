#pragma once

#include "StyledWidgets.h"

#include "Lumina/Core/Input.h"

#include <vector>

namespace KeyActions::UI
{
    inline bool ButtonStartRecording(bool enabled = true)
    {
        if (!enabled) ImGui::BeginDisabled();
        bool result = ButtonSuccess("Start Recording", Sizes::ButtonFull);
        if (!enabled) ImGui::EndDisabled();
        return result;
    }

    inline bool ButtonStopRecording(bool enabled = true)
    {
        if (!enabled) ImGui::BeginDisabled();
        bool result = ButtonDanger("Stop Recording", Sizes::ButtonFull);
        if (!enabled) ImGui::EndDisabled();
        return result;
    }

    inline bool ButtonPlay(bool enabled = true)
    {
        if (!enabled) ImGui::BeginDisabled();
        bool result = ButtonSuccess("Play", Sizes::ButtonMedium);
        if (!enabled) ImGui::EndDisabled();
        return result;
    }

    inline bool ButtonStop(bool enabled = true)
    {
        if (!enabled) ImGui::BeginDisabled();
        bool result = ButtonDanger("Stop", Sizes::ButtonMedium);
        if (!enabled) ImGui::EndDisabled();
        return result;
    }

    inline bool ButtonPause(bool enabled = true)
    {
        if (!enabled) ImGui::BeginDisabled();
        bool result = ButtonWarning("Pause", Sizes::ButtonMedium);
        if (!enabled) ImGui::EndDisabled();
        return result;
    }

    inline bool ButtonResume(bool enabled = true)
    {
        if (!enabled) ImGui::BeginDisabled();
        bool result = ButtonPrimary("Resume", Sizes::ButtonMedium);
        if (!enabled) ImGui::EndDisabled();
        return result;
    }

    inline bool ButtonSaveSettings(bool enabled = true)
    {
        if (!enabled) ImGui::BeginDisabled();
        bool result = ButtonSuccess("Save Settings", Sizes::ButtonWide);
        if (!enabled) ImGui::EndDisabled();
        return result;
    }

    inline bool ButtonRevertSettings(bool enabled = true)
    {
        if (!enabled) ImGui::BeginDisabled();
        bool result = ButtonDanger("Revert All", Sizes::ButtonWide);
        if (!enabled) ImGui::EndDisabled();
        return result;
    }

    inline bool ButtonBrowse()
    {
        return ButtonPrimary("Browse...", Sizes::ButtonWide);
    }

    inline bool ButtonRefresh()
    {
        return ButtonMuted("Refresh List", Sizes::ButtonSmall);
    }

    inline bool ButtonClear()
    {
        return ButtonDanger("Clear Events", Sizes::ButtonFull);
    }

    inline bool ButtonKeybind(const char* label, bool isCapturing, const std::vector<Lumina::KeyCode>& keys = {})
    {
        if (isCapturing)
        {
            std::string text = "Press keys... (ESC to cancel)";

            if (!keys.empty())
            {
                text = "";
                for (size_t i = 0; i < keys.size(); i++)
                {
                    text += Lumina::Input::KeyCodeToString(keys[i]);
                    if (i < keys.size() - 1)
                        text += " + ";
                }
            }

            return ButtonWarning(text.c_str(), Sizes::ButtonWide);
        }
        else
        {
            return ButtonPrimary(label, Sizes::ButtonWide);
        }
    }

    inline void DisplayKeybind(const std::vector<Lumina::KeyCode>& keys)
    {
        if (keys.empty())
        {
            TextMuted("Not assigned");
            return;
        }

        ImGui::SameLine();
        ImGui::Text(": ");
        ImGui::SameLine();

        std::string keysText;
        for (size_t i = 0; i < keys.size(); i++)
        {
            keysText += Lumina::Input::KeyCodeToString(keys[i]);
            if (i < keys.size() - 1)
                keysText += " + ";
        }

        TextHighlight("%s", keysText.c_str());
    }

    inline void DisplayKeybindCapturing(const std::vector<Lumina::KeyCode>& keys)
    {
        if (keys.empty())
            return;

        ImGui::SameLine();
        ImGui::Text(": ");
        ImGui::SameLine();

        std::string keysText;
        for (size_t i = 0; i < keys.size(); i++)
        {
            keysText += Lumina::Input::KeyCodeToString(keys[i]);
            if (i < keys.size() - 1)
                keysText += " + ";
        }

        TextSuccess("%s", keysText.c_str());
    }

    inline void StatusIndicator(const char* label, bool active, const char* activeText = "ACTIVE", const char* inactiveText = "INACTIVE")
    {
        Label(label);
        ImGui::SameLine();

        if (active)
            TextSuccess("%s", activeText);
        else
            TextMuted("%s", inactiveText);
    }
}
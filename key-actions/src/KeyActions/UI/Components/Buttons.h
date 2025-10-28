#pragma once

#include "KeyActions/UI/Styles/Theme.h"

#include <algorithm>
#include <string>

#include <imgui.h>

namespace KeyActions::UI
{
    inline bool ButtonPrimary(std::string_view text, const ImVec2& size = Sizes::ButtonMedium, bool enabled = true)
    {
        if (!enabled) ImGui::BeginDisabled();

        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Primary);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::PrimaryHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::PrimaryActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(text.data(), size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

		if (!enabled) ImGui::EndDisabled();

        return result;
    }

    inline bool ButtonSuccess(std::string_view text, const ImVec2& size = Sizes::ButtonMedium, bool enabled = true)
    {
		if (!enabled) ImGui::BeginDisabled();

        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Success);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::SuccessHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::SuccessActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(text.data(), size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

		if (!enabled) ImGui::EndDisabled();

        return result;
    }

    inline bool ButtonDanger(std::string_view text, const ImVec2& size = Sizes::ButtonMedium, bool enabled = true)
    {
		if (!enabled) ImGui::BeginDisabled();

        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Danger);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::DangerHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::DangerActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(text.data(), size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

		if (!enabled) ImGui::EndDisabled();

        return result;
    }

    inline bool ButtonWarning(std::string_view text, const ImVec2& size = Sizes::ButtonMedium, bool enabled = true)
    {
		if (!enabled) ImGui::BeginDisabled();

        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Warning);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::WarningHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::WarningActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(text.data(), size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

		if (!enabled) ImGui::EndDisabled();

        return result;
    }

    inline bool ButtonMuted(std::string_view text, const ImVec2& size = Sizes::ButtonMedium, bool enabled = true)
    {
		if (!enabled) ImGui::BeginDisabled();

        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Muted);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::MutedHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::MutedActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(text.data(), size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

		if (!enabled) ImGui::EndDisabled();

        return result;
    }

    inline bool ButtonColored(std::string_view text, const ImVec4& color, const ImVec2& size = Sizes::ButtonMedium, bool enabled = true)
    {
		if (!enabled) ImGui::BeginDisabled();

        float r = std::min(color.x * 1.2f, 1.0f);
        float g = std::min(color.y * 1.2f, 1.0f);
        float b = std::min(color.z * 1.2f, 1.0f);
        ImVec4 hoverColor = ImVec4(r, g, b, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(text.data(), size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

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
}
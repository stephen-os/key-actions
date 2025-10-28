#pragma once

#include "KeyActions/UI/Styles/Theme.h"

#include <string_view>
#include <imgui.h>

namespace KeyActions::UI
{
    inline void ProgressBar(float fraction, const ImVec2& size = ImVec2(-1, 0), std::string_view overlay = "")
    {
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Colors::Primary);
        ImGui::ProgressBar(fraction, size, overlay.empty() ? nullptr : overlay.data());
        ImGui::PopStyleColor();
    }

    inline bool Selectable(std::string_view text, bool selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0))
    {
        if (selected)
        {
            ImGui::PushStyleColor(ImGuiCol_Header, Colors::Primary);
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Colors::PrimaryHover);
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, Colors::PrimaryActive);
        }

        bool result = ImGui::Selectable(text.data(), selected, flags, size);

        if (selected)
        {
            ImGui::PopStyleColor(3);
        }

        return result;
    }
}
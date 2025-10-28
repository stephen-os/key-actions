#pragma once

#include "KeyActions/UI/Styles/Theme.h"

#include <string_view>
#include <imgui.h>

namespace KeyActions::UI
{
    inline void BeginPanel(std::string_view label, const ImVec2& size = ImVec2(0, 0), bool border = true)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, Rounding::Medium);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors::Panel);
        ImGui::PushStyleColor(ImGuiCol_Border, Colors::PanelBorder);
        ImGui::BeginChild(label.data(), size, border);
    }

    inline void EndPanel()
    {
        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
    }

    inline void SectionSeparator()
    {
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Separator, Colors::Separator);
        ImGui::Separator();
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    inline void Spacing()
    {
        ImGui::Spacing();
    }

    inline void SameLine()
    {
        ImGui::SameLine();
    }
}
#pragma once

#include "KeyActions/UI/Styles/Theme.h"

#include <string>

#include <imgui.h>

namespace KeyActions::UI
{
    inline void HeadingOne(std::string_view text)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextHighlight);
        ImGui::Text("%s", text.data());
        ImGui::PopStyleColor();
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Separator, Colors::Primary);
        ImGui::Separator();
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    inline void HeadingTwo(std::string_view text)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextNormal);
        ImGui::Text("%s", text.data());
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    inline void Label(std::string_view text)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextMuted);
        ImGui::Text("%s", text.data());
        ImGui::PopStyleColor();
    }
}
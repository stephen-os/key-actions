#pragma once

#include "KeyActions/UI/Styles/Theme.h"

#include <string_view>
#include <imgui.h>

namespace KeyActions::UI
{
    inline bool InputTextStyled(std::string_view label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Small);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors::FrameBg);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Colors::FrameBgHover);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Colors::FrameBgActive);

        bool result = ImGui::InputText(label.data(), buf, buf_size, flags);

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        return result;
    }

    inline bool CheckboxStyled(std::string_view label, bool* v)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Small);
        ImGui::PushStyleColor(ImGuiCol_CheckMark, Colors::CheckMark);

        bool result = ImGui::Checkbox(label.data(), v);

        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        return result;
    }

    inline bool SliderIntStyled(std::string_view label, int* v, int min, int max, std::string_view format = "%d")
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Small);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, Colors::SliderGrab);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, Colors::SliderGrabActive);

        bool result = ImGui::SliderInt(label.data(), v, min, max, format.data());

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        return result;
    }

    inline bool SliderFloatStyled(std::string_view label, float* v, float min, float max, std::string_view format = "%.3f")
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Small);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, Colors::SliderGrab);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, Colors::SliderGrabActive);

        bool result = ImGui::SliderFloat(label.data(), v, min, max, format.data());

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        return result;
    }
}
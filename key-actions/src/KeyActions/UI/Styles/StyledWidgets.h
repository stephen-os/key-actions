#pragma once
#include "Theme.h"
#include <imgui.h>
#include <string>

namespace KeyActions::UI
{
    inline bool ButtonPrimary(const char* label, const ImVec2& size = Sizes::ButtonMedium)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Primary);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::PrimaryHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::PrimaryActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(label, size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        return result;
    }

    inline bool ButtonSuccess(const char* label, const ImVec2& size = Sizes::ButtonMedium)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Success);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::SuccessHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::SuccessActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(label, size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        return result;
    }

    inline bool ButtonDanger(const char* label, const ImVec2& size = Sizes::ButtonMedium)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Danger);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::DangerHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::DangerActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(label, size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        return result;
    }

    inline bool ButtonWarning(const char* label, const ImVec2& size = Sizes::ButtonMedium)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Warning);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::WarningHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::WarningActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(label, size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        return result;
    }

    inline bool ButtonMuted(const char* label, const ImVec2& size = Sizes::ButtonMedium)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Muted);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::MutedHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::MutedActive);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(label, size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        return result;
    }

    inline bool ButtonColored(const char* label, const ImVec4& color, const ImVec2& size = Sizes::ButtonMedium)
    {
        ImVec4 hoverColor = ImVec4(
            std::min(color.x * 1.2f, 1.0f),
            std::min(color.y * 1.2f, 1.0f),
            std::min(color.z * 1.2f, 1.0f),
            1.0f
        );

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Medium);

        bool result = ImGui::Button(label, size);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        return result;
    }

    inline void TextSuccess(const char* fmt, ...)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextSuccess);
        va_list args;
        va_start(args, fmt);
        ImGui::TextV(fmt, args);
        va_end(args);
        ImGui::PopStyleColor();
    }

    inline void TextDanger(const char* fmt, ...)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextDanger);
        va_list args;
        va_start(args, fmt);
        ImGui::TextV(fmt, args);
        va_end(args);
        ImGui::PopStyleColor();
    }

    inline void TextWarning(const char* fmt, ...)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextWarning);
        va_list args;
        va_start(args, fmt);
        ImGui::TextV(fmt, args);
        va_end(args);
        ImGui::PopStyleColor();
    }

    inline void TextMuted(const char* fmt, ...)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextMuted);
        va_list args;
        va_start(args, fmt);
        ImGui::TextV(fmt, args);
        va_end(args);
        ImGui::PopStyleColor();
    }

    inline void TextHighlight(const char* fmt, ...)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextHighlight);
        va_list args;
        va_start(args, fmt);
        ImGui::TextV(fmt, args);
        va_end(args);
        ImGui::PopStyleColor();
    }

    inline void Heading1(const char* text)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextHighlight);
        ImGui::Text("%s", text);
        ImGui::PopStyleColor();
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Separator, Colors::Primary);
        ImGui::Separator();
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    inline void Heading2(const char* text)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextNormal);
        ImGui::Text("%s", text);
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    inline void Label(const char* text)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextMuted);
        ImGui::Text("%s", text);
        ImGui::PopStyleColor();
    }

    inline bool InputTextStyled(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Small);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors::FrameBg);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Colors::FrameBgHover);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Colors::FrameBgActive);

        bool result = ImGui::InputText(label, buf, buf_size, flags);

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        return result;
    }

    inline bool CheckboxStyled(const char* label, bool* v)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Small);
        ImGui::PushStyleColor(ImGuiCol_CheckMark, Colors::CheckMark);

        bool result = ImGui::Checkbox(label, v);

        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        return result;
    }

    inline bool SliderIntStyled(const char* label, int* v, int min, int max, const char* format = "%d")
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Small);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, Colors::SliderGrab);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, Colors::SliderGrabActive);

        bool result = ImGui::SliderInt(label, v, min, max, format);

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        return result;
    }

    inline bool SliderFloatStyled(const char* label, float* v, float min, float max, const char* format = "%.3f")
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Small);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, Colors::SliderGrab);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, Colors::SliderGrabActive);

        bool result = ImGui::SliderFloat(label, v, min, max, format);

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        return result;
    }

    inline void BeginPanel(const char* label, const ImVec2& size = ImVec2(0, 0), bool border = true)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, Rounding::Medium);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors::Panel);
        ImGui::PushStyleColor(ImGuiCol_Border, Colors::PanelBorder);
        ImGui::BeginChild(label, size, border);
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

    inline void ProgressBarStyled(float fraction, const ImVec2& size = ImVec2(-1, 0), const char* overlay = nullptr)
    {
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Colors::Primary);
        ImGui::ProgressBar(fraction, size, overlay);
        ImGui::PopStyleColor();
    }

    inline bool SelectableStyled(const char* label, bool selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0))
    {
        if (selected)
        {
            ImGui::PushStyleColor(ImGuiCol_Header, Colors::Primary);
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Colors::PrimaryHover);
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, Colors::PrimaryActive);
        }

        bool result = ImGui::Selectable(label, selected, flags, size);

        if (selected)
            ImGui::PopStyleColor(3);

        return result;
    }

    inline void Spacing()
    {
        ImGui::Spacing();
    }
}
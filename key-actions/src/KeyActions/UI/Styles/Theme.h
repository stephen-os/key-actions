#pragma once
#include <imgui.h>

namespace KeyActions::UI
{
    namespace Colors
    {
        inline constexpr ImVec4 Background = ImVec4(0.11f, 0.11f, 0.11f, 1.0f);
        inline constexpr ImVec4 BackgroundDark = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
        inline constexpr ImVec4 BackgroundLight = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);

        inline constexpr ImVec4 Panel = ImVec4(0.13f, 0.13f, 0.13f, 1.0f);
        inline constexpr ImVec4 PanelBorder = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);

        inline constexpr ImVec4 Primary = ImVec4(0.95f, 0.50f, 0.20f, 1.0f);
        inline constexpr ImVec4 PrimaryHover = ImVec4(1.0f, 0.60f, 0.30f, 1.0f);
        inline constexpr ImVec4 PrimaryActive = ImVec4(0.85f, 0.45f, 0.15f, 1.0f);
        inline constexpr ImVec4 PrimaryDim = ImVec4(0.95f, 0.50f, 0.20f, 0.6f);

        inline constexpr ImVec4 Success = ImVec4(0.30f, 0.70f, 0.30f, 1.0f);
        inline constexpr ImVec4 SuccessHover = ImVec4(0.40f, 0.80f, 0.40f, 1.0f);
        inline constexpr ImVec4 SuccessActive = ImVec4(0.25f, 0.60f, 0.25f, 1.0f);

        inline constexpr ImVec4 Danger = ImVec4(0.80f, 0.25f, 0.25f, 1.0f);
        inline constexpr ImVec4 DangerHover = ImVec4(0.90f, 0.35f, 0.35f, 1.0f);
        inline constexpr ImVec4 DangerActive = ImVec4(0.70f, 0.20f, 0.20f, 1.0f);

        inline constexpr ImVec4 Warning = ImVec4(0.90f, 0.70f, 0.20f, 1.0f);
        inline constexpr ImVec4 WarningHover = ImVec4(1.0f, 0.80f, 0.30f, 1.0f);
        inline constexpr ImVec4 WarningActive = ImVec4(0.80f, 0.60f, 0.15f, 1.0f);

        inline constexpr ImVec4 Muted = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
        inline constexpr ImVec4 MutedHover = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
        inline constexpr ImVec4 MutedActive = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);

        inline constexpr ImVec4 TextNormal = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);
        inline constexpr ImVec4 TextMuted = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        inline constexpr ImVec4 TextDim = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
        inline constexpr ImVec4 TextHighlight = ImVec4(0.95f, 0.50f, 0.20f, 1.0f);
        inline constexpr ImVec4 TextSuccess = ImVec4(0.40f, 0.85f, 0.40f, 1.0f);
        inline constexpr ImVec4 TextDanger = ImVec4(0.95f, 0.35f, 0.35f, 1.0f);
        inline constexpr ImVec4 TextWarning = ImVec4(0.95f, 0.75f, 0.25f, 1.0f);

        inline constexpr ImVec4 KeyPressed = ImVec4(0.30f, 0.70f, 0.30f, 1.0f);
        inline constexpr ImVec4 KeyReleased = ImVec4(0.80f, 0.25f, 0.25f, 1.0f);
        inline constexpr ImVec4 MousePressed = ImVec4(0.25f, 0.55f, 0.85f, 1.0f);
        inline constexpr ImVec4 MouseReleased = ImVec4(0.65f, 0.35f, 0.75f, 1.0f);
        inline constexpr ImVec4 MouseMoved = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
        inline constexpr ImVec4 MouseScrolled = ImVec4(0.95f, 0.65f, 0.30f, 1.0f);

        inline constexpr ImVec4 ScrollbarBg = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
        inline constexpr ImVec4 ScrollbarGrab = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
        inline constexpr ImVec4 ScrollbarHover = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
        inline constexpr ImVec4 ScrollbarActive = ImVec4(0.95f, 0.50f, 0.20f, 1.0f);

        inline constexpr ImVec4 SliderGrab = ImVec4(0.95f, 0.50f, 0.20f, 1.0f);
        inline constexpr ImVec4 SliderGrabActive = ImVec4(1.0f, 0.60f, 0.30f, 1.0f);

        inline constexpr ImVec4 CheckMark = ImVec4(0.95f, 0.50f, 0.20f, 1.0f);

        inline constexpr ImVec4 FrameBg = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
        inline constexpr ImVec4 FrameBgHover = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);
        inline constexpr ImVec4 FrameBgActive = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

        inline constexpr ImVec4 TitleBg = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
        inline constexpr ImVec4 TitleBgActive = ImVec4(0.11f, 0.11f, 0.11f, 1.0f);

        inline constexpr ImVec4 Header = ImVec4(0.95f, 0.50f, 0.20f, 0.3f);
        inline constexpr ImVec4 HeaderHover = ImVec4(0.95f, 0.50f, 0.20f, 0.5f);
        inline constexpr ImVec4 HeaderActive = ImVec4(0.95f, 0.50f, 0.20f, 0.7f);

        inline constexpr ImVec4 Separator = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);

        inline constexpr ImVec4 Tab = ImVec4(0.13f, 0.13f, 0.13f, 1.0f);
        inline constexpr ImVec4 TabHover = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
        inline constexpr ImVec4 TabActive = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    }

    namespace Sizes
    {
        inline constexpr ImVec2 ButtonTiny = ImVec2(80, 28);
        inline constexpr ImVec2 ButtonSmall = ImVec2(120, 32);
        inline constexpr ImVec2 ButtonMedium = ImVec2(150, 36);
        inline constexpr ImVec2 ButtonLarge = ImVec2(200, 40);
        inline constexpr ImVec2 ButtonWide = ImVec2(250, 36);
        inline constexpr ImVec2 ButtonFull = ImVec2(-1, 36);
    }

    namespace Rounding
    {
        inline constexpr float None = 0.0f;
        inline constexpr float Small = 2.0f;
        inline constexpr float Medium = 4.0f;
        inline constexpr float Large = 6.0f;
    }

    namespace Padding
    {
        inline constexpr ImVec2 Tiny = ImVec2(4, 2);
        inline constexpr ImVec2 Small = ImVec2(8, 4);
        inline constexpr ImVec2 Medium = ImVec2(12, 6);
        inline constexpr ImVec2 Large = ImVec2(16, 8);
    }

    inline void ApplyTheme()
    {
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowPadding = ImVec2(12, 12);
        style.FramePadding = ImVec2(8, 4);
        style.ItemSpacing = ImVec2(8, 6);
        style.ItemInnerSpacing = ImVec2(6, 4);
        style.IndentSpacing = 20.0f;
        style.ScrollbarSize = 14.0f;
        style.GrabMinSize = 10.0f;

        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.TabBorderSize = 0.0f;

        style.WindowRounding = 0.0f;
        style.ChildRounding = 4.0f;
        style.FrameRounding = 3.0f;
        style.PopupRounding = 4.0f;
        style.ScrollbarRounding = 8.0f;
        style.GrabRounding = 3.0f;
        style.TabRounding = 4.0f;

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_Text] = Colors::TextNormal;
        colors[ImGuiCol_TextDisabled] = Colors::TextDim;
        colors[ImGuiCol_WindowBg] = Colors::Background;
        colors[ImGuiCol_ChildBg] = Colors::Panel;
        colors[ImGuiCol_PopupBg] = Colors::Panel;
        colors[ImGuiCol_Border] = Colors::PanelBorder;
        colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_FrameBg] = Colors::FrameBg;
        colors[ImGuiCol_FrameBgHovered] = Colors::FrameBgHover;
        colors[ImGuiCol_FrameBgActive] = Colors::FrameBgActive;
        colors[ImGuiCol_TitleBg] = Colors::TitleBg;
        colors[ImGuiCol_TitleBgActive] = Colors::TitleBgActive;
        colors[ImGuiCol_TitleBgCollapsed] = Colors::TitleBg;
        colors[ImGuiCol_MenuBarBg] = Colors::Panel;
        colors[ImGuiCol_ScrollbarBg] = Colors::ScrollbarBg;
        colors[ImGuiCol_ScrollbarGrab] = Colors::ScrollbarGrab;
        colors[ImGuiCol_ScrollbarGrabHovered] = Colors::ScrollbarHover;
        colors[ImGuiCol_ScrollbarGrabActive] = Colors::ScrollbarActive;
        colors[ImGuiCol_CheckMark] = Colors::CheckMark;
        colors[ImGuiCol_SliderGrab] = Colors::SliderGrab;
        colors[ImGuiCol_SliderGrabActive] = Colors::SliderGrabActive;
        colors[ImGuiCol_Button] = Colors::Muted;
        colors[ImGuiCol_ButtonHovered] = Colors::MutedHover;
        colors[ImGuiCol_ButtonActive] = Colors::MutedActive;
        colors[ImGuiCol_Header] = Colors::Header;
        colors[ImGuiCol_HeaderHovered] = Colors::HeaderHover;
        colors[ImGuiCol_HeaderActive] = Colors::HeaderActive;
        colors[ImGuiCol_Separator] = Colors::Separator;
        colors[ImGuiCol_SeparatorHovered] = Colors::Primary;
        colors[ImGuiCol_SeparatorActive] = Colors::PrimaryActive;
        colors[ImGuiCol_ResizeGrip] = Colors::PrimaryDim;
        colors[ImGuiCol_ResizeGripHovered] = Colors::PrimaryHover;
        colors[ImGuiCol_ResizeGripActive] = Colors::Primary;
        colors[ImGuiCol_Tab] = Colors::Tab;
        colors[ImGuiCol_TabHovered] = Colors::TabHover;
        colors[ImGuiCol_TabActive] = Colors::TabActive;
        colors[ImGuiCol_TabUnfocused] = Colors::Tab;
        colors[ImGuiCol_TabUnfocusedActive] = Colors::TabActive;
        colors[ImGuiCol_PlotLines] = Colors::Primary;
        colors[ImGuiCol_PlotLinesHovered] = Colors::PrimaryHover;
        colors[ImGuiCol_PlotHistogram] = Colors::Primary;
        colors[ImGuiCol_PlotHistogramHovered] = Colors::PrimaryHover;
        colors[ImGuiCol_TextSelectedBg] = Colors::Header;
        colors[ImGuiCol_DragDropTarget] = Colors::Primary;
        colors[ImGuiCol_NavHighlight] = Colors::Primary;
        colors[ImGuiCol_NavWindowingHighlight] = Colors::Primary;
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
    }
}
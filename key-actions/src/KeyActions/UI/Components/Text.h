#pragma once
#include "KeyActions/UI/Styles/Theme.h"
#include <string_view>
#include <string>
#include <format>
#include <imgui.h>
namespace KeyActions::UI
{
    template<typename... Args>
    inline void TextSuccess(std::format_string<Args...> fmt, Args&&... args)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextSuccess);
        if constexpr (sizeof...(args) > 0) {
            std::string formatted = std::format(fmt, std::forward<Args>(args)...);
            ImGui::TextUnformatted(formatted.c_str());
        }
        else {
            ImGui::TextUnformatted(fmt.get().data());
        }
        ImGui::PopStyleColor();
    }

    template<typename... Args>
    inline void TextDanger(std::format_string<Args...> fmt, Args&&... args)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextDanger);
        if constexpr (sizeof...(args) > 0) {
            std::string formatted = std::format(fmt, std::forward<Args>(args)...);
            ImGui::TextUnformatted(formatted.c_str());
        }
        else {
            ImGui::TextUnformatted(fmt.get().data());
        }
        ImGui::PopStyleColor();
    }

    template<typename... Args>
    inline void TextWarning(std::format_string<Args...> fmt, Args&&... args)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextWarning);
        if constexpr (sizeof...(args) > 0) {
            std::string formatted = std::format(fmt, std::forward<Args>(args)...);
            ImGui::TextUnformatted(formatted.c_str());
        }
        else {
            ImGui::TextUnformatted(fmt.get().data());
        }
        ImGui::PopStyleColor();
    }

    template<typename... Args>
    inline void TextMuted(std::format_string<Args...> fmt, Args&&... args)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextMuted);
        if constexpr (sizeof...(args) > 0) {
            std::string formatted = std::format(fmt, std::forward<Args>(args)...);
            ImGui::TextUnformatted(formatted.c_str());
        }
        else {
            ImGui::TextUnformatted(fmt.get().data());
        }
        ImGui::PopStyleColor();
    }

    template<typename... Args>
    inline void TextHighlight(std::format_string<Args...> fmt, Args&&... args)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextHighlight);
        if constexpr (sizeof...(args) > 0) {
            std::string formatted = std::format(fmt, std::forward<Args>(args)...);
            ImGui::TextUnformatted(formatted.c_str());
        }
        else {
            ImGui::TextUnformatted(fmt.get().data());
        }
        ImGui::PopStyleColor();
    }
}
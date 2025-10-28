#pragma once
#include "KeyActions/UI/Styles/Theme.h"

#include "Lumina/Core/Input.h"

#include "Buttons.h"

#include <vector>
#include <string>
#include <string_view>

#include <imgui.h>

namespace KeyActions::UI
{
    inline bool ButtonKeybind(std::string_view label, bool isCapturing, const std::vector<Lumina::KeyCode>& keys = {})
    {
        if (isCapturing)
        {
            std::string text = "Press keys... (ESC to cancel)";

            if (!keys.empty())
            {
                text.clear();
                for (size_t i = 0; i < keys.size(); i++)
                {
                    text += Lumina::Input::KeyCodeToString(keys[i]);
                    if (i < keys.size() - 1)
                        text += " + ";
                }
            }

            return ButtonWarning(text, Sizes::ButtonWide);
        }
        else
        {
            return ButtonPrimary(label, Sizes::ButtonWide);
        }
    }

    inline void DisplayKeybind(const std::vector<Lumina::KeyCode>& keys)
    {
        ImGui::SameLine();

        if (keys.empty())
        {
            ImGui::AlignTextToFramePadding();
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextMuted);
            ImGui::TextDisabled("Not assigned");
            ImGui::PopStyleColor();
            return;
        }

        const float buttonHeight = Sizes::ButtonWide.y;
        const ImVec4 keyBg = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        const ImVec4 keyBorder = Colors::PrimaryDim;
        const float keyRounding = 3.0f;
        const ImVec2 keyPadding(6, 4);
        const float keySpacing = 5.0f;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        float totalWidth = 0;

        for (size_t i = 0; i < keys.size(); i++)
        {
            std::string keyName = Lumina::Input::KeyCodeToString(keys[i]);
            ImVec2 textSize = ImGui::CalcTextSize(keyName.c_str());
            ImVec2 keySize = ImVec2(textSize.x + keyPadding.x * 2, buttonHeight * 0.8f);

            float keyOffsetY = (buttonHeight - keySize.y) * 0.5f;
            ImVec2 keyMin = ImVec2(pos.x + totalWidth, pos.y + keyOffsetY);
            ImVec2 keyMax = ImVec2(keyMin.x + keySize.x, keyMin.y + keySize.y);

            drawList->AddRectFilled(keyMin, keyMax, ImGui::ColorConvertFloat4ToU32(keyBg), keyRounding);
            drawList->AddRect(keyMin, keyMax, ImGui::ColorConvertFloat4ToU32(keyBorder), keyRounding);

            ImVec2 textPos = ImVec2(
                keyMin.x + (keySize.x - textSize.x) * 0.5f,
                keyMin.y + (keySize.y - textSize.y) * 0.5f
            );
            drawList->AddText(textPos, ImGui::ColorConvertFloat4ToU32(Colors::TextNormal), keyName.c_str());

            totalWidth += keySize.x;

            if (i < keys.size() - 1)
            {
                ImVec2 plusSize = ImGui::CalcTextSize("+");
                ImVec2 plusPos = ImVec2(
                    pos.x + totalWidth + keySpacing,
                    pos.y + (buttonHeight - plusSize.y) * 0.5f
                );
                drawList->AddText(plusPos, ImGui::ColorConvertFloat4ToU32(Colors::TextMuted), "+");
                totalWidth += plusSize.x + keySpacing * 2;
            }
        }

        ImGui::Dummy(ImVec2(totalWidth, buttonHeight));
    }

    inline void DisplayKeybindCapturing(const std::vector<Lumina::KeyCode>& keys)
    {
        if (keys.empty()) return;

        ImGui::SameLine();

        const float buttonHeight = Sizes::ButtonWide.y;
        const ImVec4 keyBg = ImVec4(0.1f, 0.3f, 0.1f, 1.0f);
        const ImVec4 keyBorder = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
        const float keyRounding = 3.0f;
        const ImVec2 keyPadding(6, 4);
        const float keySpacing = 5.0f;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        float totalWidth = 0;

        for (size_t i = 0; i < keys.size(); i++)
        {
            std::string keyName = Lumina::Input::KeyCodeToString(keys[i]);
            ImVec2 textSize = ImGui::CalcTextSize(keyName.c_str());
            ImVec2 keySize = ImVec2(textSize.x + keyPadding.x * 2, buttonHeight * 0.8f);

            float keyOffsetY = (buttonHeight - keySize.y) * 0.5f;
            ImVec2 keyMin = ImVec2(pos.x + totalWidth, pos.y + keyOffsetY);
            ImVec2 keyMax = ImVec2(keyMin.x + keySize.x, keyMin.y + keySize.y);

            drawList->AddRect(
                ImVec2(keyMin.x - 2, keyMin.y - 2),
                ImVec2(keyMax.x + 2, keyMax.y + 2),
                ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 0.0f, 0.5f)),
                keyRounding + 2, 0, 2.0f);

            drawList->AddRectFilled(keyMin, keyMax, ImGui::ColorConvertFloat4ToU32(keyBg), keyRounding);
            drawList->AddRect(keyMin, keyMax, ImGui::ColorConvertFloat4ToU32(keyBorder), keyRounding);

            ImVec2 textPos = ImVec2(
                keyMin.x + (keySize.x - textSize.x) * 0.5f,
                keyMin.y + (keySize.y - textSize.y) * 0.5f
            );
            drawList->AddText(textPos, ImGui::ColorConvertFloat4ToU32(Colors::TextSuccess), keyName.c_str());

            totalWidth += keySize.x;

            if (i < keys.size() - 1)
            {
                ImVec2 plusSize = ImGui::CalcTextSize("+");
                ImVec2 plusPos = ImVec2(
                    pos.x + totalWidth + keySpacing,
                    pos.y + (buttonHeight - plusSize.y) * 0.5f
                );
                drawList->AddText(plusPos, ImGui::ColorConvertFloat4ToU32(Colors::TextSuccess), "+");
                totalWidth += plusSize.x + keySpacing * 2;
            }
        }

        ImGui::Dummy(ImVec2(totalWidth, buttonHeight));
    }
}
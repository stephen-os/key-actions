#pragma once

#include "KeyActions/UI/Styles/Theme.h"

#include <ImGuiFileDialog.h>
#include <imgui.h>

namespace KeyActions
{
    namespace UI
    {
        inline bool FileDialogFolder(std::string_view dialogId, std::string_view title, std::string& currentPath)
        {
            bool pathChanged = false;

            if (ImGui::Button("Browse...", Sizes::ButtonMedium))
            {
                IGFD::FileDialogConfig config;
                config.path = currentPath;
                config.flags = ImGuiFileDialogFlags_Modal;
                ImGuiFileDialog::Instance()->OpenDialog(dialogId.data(), title.data(), nullptr, config);
            }

            if (ImGuiFileDialog::Instance()->Display(dialogId.data(), ImGuiWindowFlags_NoResize, Sizes::FileDialogMedium, Sizes::FileDialogMedium))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    std::string newPath = ImGuiFileDialog::Instance()->GetFilePathName();
                    currentPath = newPath;
                    pathChanged = true;
                }
                ImGuiFileDialog::Instance()->Close();
            }

            return pathChanged;
        }
    }
}
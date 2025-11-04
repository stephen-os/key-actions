// NodeEditorTab.h
#pragma once

#include "Tab.h"
#include <imgui.h>
#include <imgui_node_editor.h>
#include <vector>
#include <string>

namespace ed = ax::NodeEditor;

namespace KeyActions
{
    class NodeEditorTab : public Tab
    {
    public:
        // Basic link structure
        struct LinkInfo
        {
            ed::LinkId Id;
            ed::PinId  InputId;
            ed::PinId  OutputId;
        };

    public:
        NodeEditorTab();
        virtual ~NodeEditorTab();

        // Tab interface implementation
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(float timestep) override;
        virtual void OnEvent(Event& e) override;
        virtual void OnRender() override;

    private:
        // Helper functions from the example
        void ImGuiEx_BeginColumn();
        void ImGuiEx_NextColumn();
        void ImGuiEx_EndColumn();

    private:
        ed::EditorContext* m_Context = nullptr;
        bool m_FirstFrame = true;
        std::vector<LinkInfo> m_Links;
        int m_NextLinkId = 100;
    };
}
// NodeEditorTab.cpp
#include "NodeEditorTab.h"
#include <imgui.h>
#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;

namespace KeyActions
{
    NodeEditorTab::NodeEditorTab()
        : Tab("Node Editor")
    {
    }

    NodeEditorTab::~NodeEditorTab()
    {
        if (m_Context)
        {
            ed::DestroyEditor(m_Context);
            m_Context = nullptr;
        }
    }

    void NodeEditorTab::OnAttach()
    {
        // Create and configure the editor context
        ed::Config config;
        config.SettingsFile = "BasicInteraction.json";
        m_Context = ed::CreateEditor(&config);
    }

    void NodeEditorTab::OnDetach()
    {
        if (m_Context)
        {
            ed::DestroyEditor(m_Context);
            m_Context = nullptr;
        }

        // Clear data
        m_Links.clear();
    }

    void NodeEditorTab::OnUpdate(float timestep)
    {
        // No updates needed for this simple example
    }

    void NodeEditorTab::OnEvent(Event& e)
    {
        // No events to handle for this simple example
    }

    void NodeEditorTab::ImGuiEx_BeginColumn()
    {
        ImGui::BeginGroup();
    }

    void NodeEditorTab::ImGuiEx_NextColumn()
    {
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
    }

    void NodeEditorTab::ImGuiEx_EndColumn()
    {
        ImGui::EndGroup();
    }

    void NodeEditorTab::OnRender()
    {
        if (!IsVisible())
            return;

        // Get the content region size of the current ImGui context
        ImVec2 contentSize = ImGui::GetContentRegionAvail();

        // Create a child window/frame to contain the node editor
        // This doesn't create a new window but defines a region in the current window
        if (ImGui::BeginChild(m_Name.c_str(), contentSize, true))
        {
            // Start interaction with editor
            ed::SetCurrentEditor(m_Context);

            // Use the full size of the child window
            ed::Begin("My Editor", ImGui::GetContentRegionAvail());

            int uniqueId = 1;

            // 1) Commit known data to editor

            // Submit Node A
            ed::NodeId nodeA_Id = uniqueId++;
            ed::PinId  nodeA_InputPinId = uniqueId++;
            ed::PinId  nodeA_OutputPinId = uniqueId++;

            if (m_FirstFrame)
                ed::SetNodePosition(nodeA_Id, ImVec2(0, 0));

            ed::BeginNode(nodeA_Id);
            ImGui::Text("Node A");
            ed::BeginPin(nodeA_InputPinId, ed::PinKind::Input);
            ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ed::BeginPin(nodeA_OutputPinId, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();
            ed::EndNode();

            // Submit Node B
            ed::NodeId nodeB_Id = uniqueId++;
            ed::PinId  nodeB_InputPinId1 = uniqueId++;
            ed::PinId  nodeB_InputPinId2 = uniqueId++;
            ed::PinId  nodeB_OutputPinId = uniqueId++;

            if (m_FirstFrame)
                ed::SetNodePosition(nodeB_Id, ImVec2(210, 60));

            ed::BeginNode(nodeB_Id);
            ImGui::Text("Node B");
            ImGuiEx_BeginColumn();
            ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input);
            ImGui::Text("-> In1");
            ed::EndPin();
            ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input);
            ImGui::Text("-> In2");
            ed::EndPin();
            ImGuiEx_NextColumn();
            ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();
            ImGuiEx_EndColumn();
            ed::EndNode();

            // Submit Links
            for (auto& linkInfo : m_Links)
                ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

            // 2) Handle interactions

            // Handle creation action
            if (ed::BeginCreate())
            {
                ed::PinId inputPinId, outputPinId;
                if (ed::QueryNewLink(&inputPinId, &outputPinId))
                {
                    if (inputPinId && outputPinId) // both are valid, let's accept link
                    {
                        if (ed::AcceptNewItem())
                        {
                            // Add new link
                            m_Links.push_back({ ed::LinkId(m_NextLinkId++), inputPinId, outputPinId });

                            // Draw new link
                            ed::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
                        }
                    }
                }
            }
            ed::EndCreate();

            // Handle deletion action
            if (ed::BeginDelete())
            {
                ed::LinkId deletedLinkId;
                while (ed::QueryDeletedLink(&deletedLinkId))
                {
                    if (ed::AcceptDeletedItem())
                    {
                        // Remove link from your data
                        for (auto it = m_Links.begin(); it != m_Links.end(); ++it)
                        {
                            if (it->Id == deletedLinkId)
                            {
                                m_Links.erase(it);
                                break;
                            }
                        }
                    }
                }
            }
            ed::EndDelete();

            // End of interaction with editor
            ed::End();

            // Center the view on the nodes when first rendering
            if (m_FirstFrame)
            {
                // Navigate to content with padding
                ed::NavigateToContent(0.1f);
                m_FirstFrame = false;
            }

            ed::SetCurrentEditor(nullptr);
        }
        ImGui::EndChild();
    }
}
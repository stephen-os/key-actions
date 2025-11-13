#pragma once

#include <string>
#include <unordered_map>
#include "Lumina/Core/Base.h"
#include "Lumina/Utils/UUID.h"
#include "Lumina/Input/GlobalInputPlayback.h"
#include <imgui_node_editor.h>
#include <glm/vec2.hpp>

namespace NE = ax::NodeEditor;

#define NODE_ID_NONE 0
#define PIN_ID_NONE 0
#define LINK_ID_NONE 0

namespace KeyActions
{
    enum class NodeType
    {
        Start = 0,
        KeyPress,
        KeyRelease,

		MouseMove,
		MousePress,
		MouseRelease,
		MouseScroll,
    };

    enum class PinType
    {
        Undefined = 0,
        Input,
        Output,
    };

    class Node
    {
    public:
        using NodePtr = Lumina::Ref<Node>;

        struct Pin
        {
            NE::PinId Id = PIN_ID_NONE;
            NE::LinkId LinkId = LINK_ID_NONE;
            std::string Name = "*Unnamed Pin*";
            PinType Type = PinType::Undefined;
            NodePtr ConnectedNode = nullptr;
        };

        Node(const std::string& name) : m_Name(name), m_NodeId(Lumina::UUID::Generate()) {}
        virtual ~Node()
        {
            for (auto& pin : m_Pins)
            {
                pin.ConnectedNode.reset();
            }
        }

        virtual NodePtr Execute(Lumina::GlobalInputPlayback* playback) = 0;
        virtual NodeType GetType() const = 0;

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const { return m_Name; }
        
        const NE::NodeId& GetNodeID() const { return m_NodeId; }
       
        virtual bool CanConnect(PinType sourceType, PinType targetType) = 0;

        std::vector<Pin>& GetPins() { return m_Pins; }

        Pin* GetPin(const NE::PinId& id)
        {
            for (auto& pin : m_Pins)
            {
                if (pin.Id == id)
                    return &pin;
            }
            return nullptr;
        }

        Pin* GetPin(const NE::LinkId& linkId)
        {
            for (auto& pin : m_Pins)
            {
                if (pin.LinkId == linkId)
                    return &pin;
            }
            return nullptr;
        }

        Pin* GetPin(PinType type)
        {
            for (auto& pin : m_Pins)
            {
                if (pin.Type == type)
                    return &pin;
            }
            return nullptr;
        }

        bool HasPin(PinType type) const
        {
            for (const auto& pin : m_Pins)
            {
                if (pin.Type == type)
                    return true;
            }
            return false;
        }

        static NE::LinkId ConnectPin(NodePtr fromNode, PinType fromPinType, NodePtr toNode, PinType toPinType)
        {
			LUMINA_ASSERT(fromNode != nullptr, "ConnectPin: fromNode is null");
            LUMINA_ASSERT(toNode != nullptr, "ConnectPin: toNode is null");

			// Do both nodes have the specified pins?
            if (fromNode->HasPin(fromPinType) == false || toNode->HasPin(toPinType) == false)
				return NE::LinkId(LINK_ID_NONE);

			// Can we connect these pin types?
            if (!(fromNode->CanConnect(fromPinType, toPinType) && toNode->CanConnect(toPinType, fromPinType)))
				return NE::LinkId(LINK_ID_NONE);

			Pin* fromNodePin = fromNode->GetPin(fromPinType);
            Pin* toNodePin = toNode->GetPin(toPinType);
			LUMINA_ASSERT(fromNodePin != nullptr, "ConnectPin: fromNodePin is null after HasPin check");
			LUMINA_ASSERT(toNodePin != nullptr, "ConnectPin: toNodePin is null after HasPin check");

			// We must disconnect existing connections first
            if (fromNodePin->ConnectedNode && fromNodePin->LinkId.Get() != LINK_ID_NONE)
                DisconnectPin(fromNode, fromPinType);

			if (toNodePin->ConnectedNode && toNodePin->LinkId.Get() != LINK_ID_NONE)
				DisconnectPin(toNode, toPinType);

            auto linkId = NE::LinkId(Lumina::UUID::Generate());

			fromNodePin->ConnectedNode = toNode;
			fromNodePin->LinkId = linkId;

			toNodePin->ConnectedNode = fromNode;
            toNodePin->LinkId = linkId;
            
            LUMINA_LOG_INFO("===================== Connection Established =====================");
            LUMINA_LOG_INFO("Link ID:               {}", linkId.Get());
            LUMINA_LOG_INFO("Source Node Name:      {}", fromNode->GetName());
            LUMINA_LOG_INFO("Source Node ID:        {}", fromNode->GetNodeID().Get());
            LUMINA_LOG_INFO("Source Pin Name:       {}", fromNodePin->Name);
            LUMINA_LOG_INFO("Source Pin ID:         {}", fromNodePin->Id.Get());
            LUMINA_LOG_INFO("Source Pin Link ID:    {}", fromNodePin->LinkId.Get());
            LUMINA_LOG_INFO("Target Node Name:      {}", toNode->GetName());
            LUMINA_LOG_INFO("Target Node ID:        {}", toNode->GetNodeID().Get());
            LUMINA_LOG_INFO("Target Pin Name:       {}", toNodePin->Name);
            LUMINA_LOG_INFO("Target Pin ID:         {}", toNodePin->Id.Get());
            LUMINA_LOG_INFO("Target Pin Link ID:    {}", toNodePin->LinkId.Get());

			return linkId;
        }

        static bool DisconnectPin(NodePtr fromNode, PinType fromPinType)
        {
            LUMINA_ASSERT(fromNode != nullptr, "DisconnectPin: node is null");

            Node::Pin* sourcePin = fromNode->GetPin(fromPinType);
            if (!sourcePin || !sourcePin->ConnectedNode)
                return false;

            NodePtr targetNode = sourcePin->ConnectedNode;
            NE::LinkId linkId = sourcePin->LinkId;

            Pin* targetPin = nullptr;
            for (auto& pin : targetNode->GetPins())
            {
                if (pin.LinkId == sourcePin->LinkId)
                {
                    targetPin = &pin;
                    break;
                }
            }

            LUMINA_ASSERT(targetPin != nullptr, "Disconnecting pin with no matching pin on the connected node!");

            targetPin->ConnectedNode = nullptr;
            targetPin->LinkId = LINK_ID_NONE;

            sourcePin->ConnectedNode = nullptr;
            sourcePin->LinkId = LINK_ID_NONE;

            LUMINA_LOG_INFO("===================== Connection Removed =====================");
            LUMINA_LOG_INFO("Link ID:               {}", linkId.Get());
            LUMINA_LOG_INFO("Source Node Name:      {}", fromNode->GetName());
            LUMINA_LOG_INFO("Source Node ID:        {}", fromNode->GetNodeID().Get());
            LUMINA_LOG_INFO("Source Pin Name:       {}", sourcePin->Name);
            LUMINA_LOG_INFO("Source Pin ID:         {}", sourcePin->Id.Get());
            LUMINA_LOG_INFO("Source Pin Link ID:    {}", LINK_ID_NONE);
            LUMINA_LOG_INFO("Target Node Name:      {}", targetNode->GetName());
            LUMINA_LOG_INFO("Target Node ID:        {}", targetNode->GetNodeID().Get());
            LUMINA_LOG_INFO("Target Pin Name:       {}", targetPin->Name);
            LUMINA_LOG_INFO("Target Pin ID:         {}", targetPin->Id.Get());
            LUMINA_LOG_INFO("Target Pin Link ID:    {}", LINK_ID_NONE);

            return true;
        }

        void SetPosition(const glm::vec2& position) { m_Position = position; }
        glm::vec2 GetPosition() const { return m_Position; }

    protected:
        Pin CreatePin(const std::string& name, const PinType type)
        {
            Pin pin;
            pin.Id = Lumina::UUID::Generate();
            pin.Type = type;
            pin.Name = name;
            return pin;
        }

        bool AddPin(const Pin& pin)
        {
            m_Pins.push_back(pin);
            return true;
        }

    protected:
        std::string m_Name = "*Unnamed Node*";
        NE::NodeId m_NodeId = NODE_ID_NONE;

        std::vector<Pin> m_Pins;
        
        glm::vec2 m_Position = { 0.0f, 0.0f };
    };
}
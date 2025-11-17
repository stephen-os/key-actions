#pragma once

#include <string>
#include <unordered_map>

#include "Lumina/Utils/UUID.h"
#include "Lumina/Input/GlobalInputPlayback.h"

#include "KeyActions/Core/Memory.h"

#include <imgui_node_editor.h>
#include <glm/vec2.hpp>

namespace KeyActions
{
	using NodeID = ax::NodeEditor::NodeId;
	using PinID = ax::NodeEditor::PinId;
	using LinkID = ax::NodeEditor::LinkId;

    inline constexpr uint64_t NODE_ID_NONE = 0;
    inline constexpr uint64_t PIN_ID_NONE = 0;
    inline constexpr uint64_t LINK_ID_NONE = 0;

    enum class NodeType
    {
        Start = 0,
        End,

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
        struct Pin
        {
            PinID Id = PIN_ID_NONE;
            LinkID LinkId = LINK_ID_NONE;
            std::string Name = "*Unnamed Pin*";
            PinType Type = PinType::Undefined;
            Ref<Node> ConnectedNode = nullptr;
        };

        Node(const std::string& name) : m_Name(name), m_NodeId(Lumina::UUID::Generate()) {}
        virtual ~Node()
        {
            for (auto& pin : m_Pins)
            {
                pin.ConnectedNode.reset();
            }
        }

        virtual Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) = 0;
        virtual NodeType GetType() const = 0;

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const { return m_Name; }
        
        const NodeID& GetNodeID() const { return m_NodeId; }
       
        virtual bool CanConnect(PinType sourceType, PinType targetType) = 0;

        std::vector<Pin>& GetPins() { return m_Pins; }

        Pin* GetPin(const PinID& id)
        {
            for (auto& pin : m_Pins)
            {
                if (pin.Id == id)
                    return &pin;
            }
            return nullptr;
        }

        Pin* GetPin(const LinkID& linkId)
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

        static bool ConnectPins(Ref<Node> nodeA, PinType pinAType , Ref<Node> nodeB, PinType pinBType)
        {
			LUMINA_ASSERT(nodeA != nullptr, "ConnectPins: nodeA is null");
            LUMINA_ASSERT(nodeB != nullptr, "ConnectPins: nodeB is null");

			// Are the the same node?
            if (nodeA->GetNodeID() == nodeB->GetNodeID())
				return false;

			// Do both nodes have the specified pins?
            if (nodeA->HasPin(pinAType) == false || nodeB->HasPin(pinBType) == false)
				return false;

			// Can we connect these pin types?
            if (!(nodeA->CanConnect(pinAType, pinBType) && nodeB->CanConnect(pinBType, pinAType)))
				return false;

			Pin* pinA = nodeA->GetPin(pinAType);
            Pin* pinB = nodeB->GetPin(pinBType);
			LUMINA_ASSERT(pinA != nullptr, "ConnectPins: pinA is null after HasPin check");
			LUMINA_ASSERT(pinB != nullptr, "ConnectPins: pinB is null after HasPin check");

			// Are the pins already connected?
            if (pinA->LinkId.Get() != LINK_ID_NONE && pinA->LinkId == pinB->LinkId)
                return true;

			// We must disconnect existing connections first
			DisconnectPin(nodeA, pinAType);
			DisconnectPin(nodeB, pinBType);

            auto linkId = LinkID(Lumina::UUID::Generate());

			pinA->ConnectedNode = nodeB;
            pinA->LinkId = linkId;

			pinB->ConnectedNode = nodeA;
            pinB->LinkId = linkId;
            
            LUMINA_LOG_INFO("===================== Connection Established =====================");
            LUMINA_LOG_INFO("Link ID:               {}", linkId.Get());
            LUMINA_LOG_INFO("Source Node Name:      {}", nodeA->GetName());
            LUMINA_LOG_INFO("Source Node ID:        {}", nodeA->GetNodeID().Get());
            LUMINA_LOG_INFO("Source Pin Name:       {}", pinA->Name);
            LUMINA_LOG_INFO("Source Pin ID:         {}", pinA->Id.Get());
            LUMINA_LOG_INFO("Source Pin Link ID:    {}", pinA->LinkId.Get());
            LUMINA_LOG_INFO("Target Node Name:      {}", nodeB->GetName());
            LUMINA_LOG_INFO("Target Node ID:        {}", nodeB->GetNodeID().Get());
            LUMINA_LOG_INFO("Target Pin Name:       {}", pinB->Name);
            LUMINA_LOG_INFO("Target Pin ID:         {}", pinB->Id.Get());
            LUMINA_LOG_INFO("Target Pin Link ID:    {}", pinB->LinkId.Get());

			return true;
        }

        static bool DisconnectPin(Ref<Node> node, PinType pinType)
        {
            LUMINA_ASSERT(node != nullptr, "DisconnectPin: node is null");

            Pin* sourcePin = node->GetPin(pinType);
            if (!sourcePin || !sourcePin->ConnectedNode || sourcePin->LinkId.Get() == LINK_ID_NONE)
                return false;

            Ref<Node> targetNode = sourcePin->ConnectedNode;
            LinkID linkId = sourcePin->LinkId;

            Pin* targetPin = nullptr;
            for (auto& p : targetNode->GetPins())
            {
                if (p.LinkId == sourcePin->LinkId)
                {
                    targetPin = &p;
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
            LUMINA_LOG_INFO("Node A Name:           {}", node->GetName());
            LUMINA_LOG_INFO("Node A ID:             {}", node->GetNodeID().Get());
            LUMINA_LOG_INFO("Pin A Name:            {}", sourcePin->Name);
            LUMINA_LOG_INFO("Pin A ID:              {}", sourcePin->Id.Get());
            LUMINA_LOG_INFO("Pin A Link ID:         {}", LINK_ID_NONE);
            LUMINA_LOG_INFO("Node B Name:           {}", targetNode->GetName());
            LUMINA_LOG_INFO("Node B ID:             {}", targetNode->GetNodeID().Get());
            LUMINA_LOG_INFO("Pin B Name:            {}", targetPin->Name);
            LUMINA_LOG_INFO("Pin B ID:              {}", targetPin->Id.Get());
            LUMINA_LOG_INFO("Pin B Link ID:         {}", LINK_ID_NONE);

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
        NodeID m_NodeId = NODE_ID_NONE;

        std::vector<Pin> m_Pins;
        
        glm::vec2 m_Position = { 0.0f, 0.0f };
    };
}
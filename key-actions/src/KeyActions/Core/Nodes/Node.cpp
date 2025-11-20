#include "Node.h"

namespace KeyActions
{
    Node::Node(const std::string& name)
        : m_Name(name), m_NodeId(Lumina::UUID::Generate())
    {
    }

    Node::~Node()
    {
        for (auto& pin : m_Pins)
        {
            pin.ConnectedNode.reset();
        }
    }

    void Node::SetName(const std::string& name)
    {
        m_Name = name;
    }

    const std::string& Node::GetName() const
    {
        return m_Name;
    }

    const NodeID& Node::GetNodeID() const
    {
        return m_NodeId;
    }

    std::vector<Node::Pin>& Node::GetPins()
    {
        return m_Pins;
    }

    size_t Node::GetPinCount() const
    {
        return m_Pins.size();
    }

    Node::Pin* Node::GetPin(const PinID& id)
    {
        for (auto& pin : m_Pins)
        {
            if (pin.Id == id)
                return &pin;
        }
        return nullptr;
    }

    Node::Pin* Node::GetPin(const LinkID& linkId)
    {
        for (auto& pin : m_Pins)
        {
            if (pin.LinkId == linkId)
                return &pin;
        }
        return nullptr;
    }

    Node::Pin* Node::GetPin(PinType type)
    {
        for (auto& pin : m_Pins)
        {
            if (pin.Type == type)
                return &pin;
        }
        return nullptr;
    }

    bool Node::HasPin(PinType type) const
    {
        for (const auto& pin : m_Pins)
        {
            if (pin.Type == type)
                return true;
        }
        return false;
    }

    bool Node::CanConnect(PinType pinTypeA, PinType pinTypeB)
    {
        if (pinTypeA == PinType::Undefined || pinTypeB == PinType::Undefined)
            return false;

        switch (pinTypeA)
        {
            case PinType::Output:
                return pinTypeB == PinType::Input;
            case PinType::Input:
                return pinTypeB == PinType::Output;
        }   
    }

    bool Node::ConnectPins(Ref<Node> nodeA, PinType pinAType, Ref<Node> nodeB, PinType pinBType)
    {
        LUMINA_ASSERT(nodeA != nullptr, "ConnectPins: nodeA is null");
        LUMINA_ASSERT(nodeB != nullptr, "ConnectPins: nodeB is null");

        if (nodeA->GetNodeID() == nodeB->GetNodeID())
            return false;

        if (!nodeA->HasPin(pinAType) || !nodeB->HasPin(pinBType))
            return false;

        if (!CanConnect(pinAType, pinBType))
            return false;

        Pin* pinA = nodeA->GetPin(pinAType);
        Pin* pinB = nodeB->GetPin(pinBType);
        LUMINA_ASSERT(pinA != nullptr, "ConnectPins: pinA is null after HasPin check");
        LUMINA_ASSERT(pinB != nullptr, "ConnectPins: pinB is null after HasPin check");

        if (pinA->LinkId.Get() != LINK_ID_NONE && pinA->LinkId == pinB->LinkId)
            return true;

        DisconnectPin(nodeA, pinAType);
        DisconnectPin(nodeB, pinBType);

        auto linkId = LinkID(Lumina::UUID::Generate());

        pinA->ConnectedNode = nodeB;
        pinA->LinkId = linkId;
        pinB->ConnectedNode = nodeA;
        pinB->LinkId = linkId;

        LUMINA_LOG_INFO("===================== Connection Established =====================");
        LUMINA_LOG_INFO("Link ID:               {}", linkId.Get());
        LUMINA_LOG_INFO("Node A Name:           {}", nodeA->GetName());
        LUMINA_LOG_INFO("Node A ID:             {}", nodeA->GetNodeID().Get());
        LUMINA_LOG_INFO("Pin A Name:            {}", pinA->Name);
        LUMINA_LOG_INFO("Pin A ID:              {}", pinA->Id.Get());
        LUMINA_LOG_INFO("Pin A Link ID:         {}", pinA->LinkId.Get());
        LUMINA_LOG_INFO("Node B Name:           {}", nodeB->GetName());
        LUMINA_LOG_INFO("Node B ID:             {}", nodeB->GetNodeID().Get());
        LUMINA_LOG_INFO("Pin B Name:            {}", pinB->Name);
        LUMINA_LOG_INFO("Pin B ID:              {}", pinB->Id.Get());
        LUMINA_LOG_INFO("Pin B Link ID:         {}", pinB->LinkId.Get());

        return true;
    }

    bool Node::DisconnectPin(Ref<Node> node, PinType pinType)
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

    void Node::SetPosition(const glm::vec2& position)
    {
        m_Position = position;
    }

    glm::vec2 Node::GetPosition() const
    {
        return m_Position;
    }

    Node::Pin Node::CreatePin(const std::string& name, const PinType type)
    {
        Pin pin;
        pin.Id = Lumina::UUID::Generate();
        pin.Type = type;
        pin.Name = name;
        return pin;
    }

    bool Node::AddPin(const Pin& pin)
    {
        m_Pins.push_back(pin);
        return true;
    }
}
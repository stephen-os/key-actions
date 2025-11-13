#include "MousePressNode.h"

#include <iostream>

namespace KeyActions
{
    Lumina::Ref<MousePressNode> Create(Lumina::MouseCode button, int x, int y)
    {
        return Lumina::CreateRef<MousePressNode>(button, x, y);
	}

    MousePressNode::MousePressNode(Lumina::MouseCode button, int x, int y) 
		: Node("MousePress"), m_Button(button), m_X(x), m_Y(y)
    {
        m_Name = "Press Mouse " + std::to_string(static_cast<int>(button)) + " at location " +
            std::to_string(x) + "," + std::to_string(y);

		AddPin(CreatePin("Input", PinType::Input));
		AddPin(CreatePin("Output", PinType::Output));
    }

    Node::NodePtr MousePressNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(playback != nullptr, "MousePressNode: Playback system is null in MousePressNode execution");
        
		playback->SimulateMouseButtonPress(m_Button, m_X, m_Y);
        
        LUMINA_LOG_INFO("Simulated mouse press of {}", static_cast<int>(m_Button));
        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;
        return nullptr;
	}

    bool MousePressNode::CanConnect(PinType sourceType, PinType targetType)
    {
        // Rule: MousePressNode allows bidirectional connections
        // 1. Output pins can connect to Input pins
        // 2. Input pins can connect to Output pins
        return ((sourceType == PinType::Output && targetType == PinType::Input) ||
            (sourceType == PinType::Input && targetType == PinType::Output));
	}

    Lumina::MouseCode MousePressNode::GetButton() const
    {
        return m_Button;
    }
}
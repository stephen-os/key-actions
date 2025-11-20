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

    Ref<Node> MousePressNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(playback != nullptr, "MousePressNode: Playback system is null in MousePressNode execution");
        
		playback->SimulateMouseButtonPress(m_Button, m_X, m_Y);
        
        LUMINA_LOG_INFO("Simulated mouse press of {}", static_cast<int>(m_Button));
        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;
        return nullptr;
	}

    Lumina::MouseCode MousePressNode::GetButton() const
    {
        return m_Button;
    }
}
#include "MouseScrollNode.h"

namespace KeyActions
{
    Lumina::Ref<MouseScrollNode> MouseScrollNode::Create(int scrollDX, int scrollDY)
    {
        return Lumina::CreateRef<MouseScrollNode>(scrollDX, scrollDY);
	}

    MouseScrollNode::MouseScrollNode(int scrollDX, int scrollDY)
        : Node("MouseScroll"), m_ScrollDX(scrollDX), m_ScrollDY(scrollDY)
    {
        m_Name = "Scroll Mouse " + std::to_string(scrollDX) + ", " + std::to_string(scrollDY);

		AddPin(CreatePin("In", PinType::Input));
		AddPin(CreatePin("Out", PinType::Output));
    }

    Ref<Node> MouseScrollNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
		LUMINA_ASSERT(playback != nullptr, "MouseScrollNode: Playback system is null in MouseScrollNode execution");

		playback->SimulateMouseScroll(m_ScrollDX, m_ScrollDY);

		LUMINA_LOG_INFO("Simulated mouse scroll of deltaX: {}, deltaY: {}", m_ScrollDX, m_ScrollDY);

        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;

        return nullptr;
	}

    bool MouseScrollNode::CanConnect(PinType sourceType, PinType targetType)
    {
        // Rule: MouseScrollNode allows bidirectional connections
        // 1. Output pins can connect to Input pins
        // 2. Input pins can connect to Output pins
        return ((sourceType == PinType::Output && targetType == PinType::Input) ||
            (sourceType == PinType::Input && targetType == PinType::Output));
	}

    int MouseScrollNode::GetScrollDX() const
    {
        return m_ScrollDX;
    }

    int MouseScrollNode::GetScrollDY() const
    {
        return m_ScrollDY;
    }
}
#include "MouseScrollNode.h"

namespace KeyActions
{
    Ref<MouseScrollNode> MouseScrollNode::Create(int scrollDX, int scrollDY)
    {
        return Lumina::CreateRef<MouseScrollNode>(scrollDX, scrollDY);
	}

    MouseScrollNode::MouseScrollNode(int scrollDX, int scrollDY) : Node("Mouse Scroll"), m_ScrollDX(scrollDX), m_ScrollDY(scrollDY)
    {
		AddPin(CreatePin("In", PinType::Input));
		AddPin(CreatePin("Out", PinType::Output));
    }

    Ref<Node> MouseScrollNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
		LUMINA_ASSERT(playback != nullptr, "MouseScrollNode: Playback system is null in MouseScrollNode execution");

		playback->SimulateMouseScroll(m_ScrollDX, m_ScrollDY);
		LUMINA_LOG_INFO("MouseScrollNode: Simulated mouse scroll of Dx: {}, Dy: {}", m_ScrollDX, m_ScrollDY);

        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;

        return nullptr;
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
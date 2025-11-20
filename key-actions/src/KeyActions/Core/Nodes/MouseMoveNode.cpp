#include "MouseMoveNode.h"

namespace KeyActions
{
    Ref<MouseMoveNode> MouseMoveNode::Create(int x, int y)
    {
        return Lumina::CreateRef<MouseMoveNode>(x, y);
    }

    MouseMoveNode::MouseMoveNode(int x, int y) : Node("Mouse Move"), m_X(x), m_Y(y)
    {
		AddPin(CreatePin("Input", PinType::Input));
		AddPin(CreatePin("Output", PinType::Output));
    }

    Ref<Node> MouseMoveNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
	    LUMINA_ASSERT(playback != nullptr, "KeyPressNode: Playback system is null in KeyPressNode execution");

		playback->SimulateMouseMove(m_X, m_Y);
		LUMINA_LOG_INFO("KeyPressNode: Simulated mouse move to ({}, {})", m_X, m_Y);

        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;

        return nullptr;
    }

    int MouseMoveNode::GetX() const
    {
        return m_X;
    }

    int MouseMoveNode::GetY() const
    {
        return m_Y;
    }
}
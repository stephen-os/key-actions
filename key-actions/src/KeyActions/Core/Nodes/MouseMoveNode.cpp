#include "MouseMoveNode.h"
#include <iostream>

namespace KeyActions
{

    MouseMoveNode::MouseMoveNode(int x, int y)
        : Node("MouseMove"), m_X(x), m_Y(y)
    {
        m_Name = "Move Mouse to " + std::to_string(x) + ", " + std::to_string(y);
    }

    Node::NodePtr MouseMoveNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
		// LUMINA_ASSERT(m_X >= 0 && m_Y >= 0, "MouseMoveNode: Invalid mouse coordinates");
        LUMINA_ASSERT(playback != nullptr, "KeyPressNode: Playback system is null in KeyPressNode execution");

		playback->SimulateMouseMove(m_X, m_Y);
		LUMINA_LOG_INFO("Simulated mouse move to ({}, {})", m_X, m_Y);

        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;

        return nullptr;
    }

    bool MouseMoveNode::CanConnect(PinType sourceType, PinType targetType)
    {
        // Rule: KeyPressNode allows bidirectional connections
        // 1. Output pins can connect to Input pins
        // 2. Input pins can connect to Output pins
        return ((sourceType == PinType::Output && targetType == PinType::Input) ||
            (sourceType == PinType::Input && targetType == PinType::Output));
    }

    int MouseMoveNode::GetX() const
    {
        return m_X;
    }

    int MouseMoveNode::GetY() const
    {
        return m_Y;
    }

    Lumina::Ref<MouseMoveNode> MouseMoveNode::Create(int x, int y)
    {
        return Lumina::CreateRef<MouseMoveNode>(x, y);
    }
}
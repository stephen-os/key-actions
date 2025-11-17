#include "MouseReleaseNode.h"

namespace KeyActions
{
    Lumina::Ref<MouseReleaseNode> MouseReleaseNode::Create(Lumina::MouseCode button, int x, int y)
    {
        return Lumina::CreateRef<MouseReleaseNode>(button, x, y);
    }

    MouseReleaseNode::MouseReleaseNode(Lumina::MouseCode button, int x, int y)
        : Node("MouseRelease"), m_Button(button), m_X(x), m_Y(y)
    {
        m_Name = "Release Mouse " + std::to_string(static_cast<int>(button)) + " at location " +
            std::to_string(x) + "," + std::to_string(y);
        
        AddPin(CreatePin("Input", PinType::Input));
        AddPin(CreatePin("Output", PinType::Output));
	}

    Ref<Node> MouseReleaseNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(playback != nullptr, "MouseReleaseNode: Playback system is null in MouseReleaseNode execution");
        
		playback->SimulateMouseButtonRelease(m_Button, m_X, m_Y);

        LUMINA_LOG_INFO("Simulated mouse release of button {}", static_cast<int>(m_Button));
        
        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;

        return nullptr;
	}

    bool MouseReleaseNode::CanConnect(PinType sourceType, PinType targetType)
    {
        // Rule: MouseReleaseNode allows bidirectional connections
        // 1. Output pins can connect to Input pins
        // 2. Input pins can connect to Output pins
        return ((sourceType == PinType::Output && targetType == PinType::Input) ||
            (sourceType == PinType::Input && targetType == PinType::Output));
    }

    Lumina::MouseCode MouseReleaseNode::GetButton() const
    {
        return m_Button;
    }

}
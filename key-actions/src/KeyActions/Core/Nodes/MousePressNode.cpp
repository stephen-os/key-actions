#include "MousePressNode.h"

#include "Lumina/Core/Input.h"

namespace KeyActions
{
    Ref<MousePressNode> MousePressNode::Create(Lumina::MouseCode button, int x, int y)
    {
        return Lumina::CreateRef<MousePressNode>(button, x, y);
	}

    MousePressNode::MousePressNode(Lumina::MouseCode button, int x, int y) : Node("Mouse Press"), m_Button(button), m_X(x), m_Y(y)
    {
		AddPin(CreatePin("Input", PinType::Input));
		AddPin(CreatePin("Output", PinType::Output));
    }

    Ref<Node> MousePressNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(playback != nullptr, "MousePressNode: Playback system is null in MousePressNode execution");
        
        playback->SimulateMouseButtonPress(m_Button, m_X, m_Y);
        LUMINA_LOG_INFO("MousePressNode: Simulated mouse press on {} button", Lumina::Input::MouseCodeToString(m_Button));

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
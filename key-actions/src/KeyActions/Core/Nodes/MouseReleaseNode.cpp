#include "MouseReleaseNode.h"

#include "Lumina/Core/Input.h"

namespace KeyActions
{
    Ref<MouseReleaseNode> MouseReleaseNode::Create(Lumina::MouseCode button, int x, int y)
    {
        return Lumina::CreateRef<MouseReleaseNode>(button, x, y);
    }

    MouseReleaseNode::MouseReleaseNode(Lumina::MouseCode button, int x, int y) : Node("Mouse Release"), m_Button(button), m_X(x), m_Y(y)
    {    
        AddPin(CreatePin("Input", PinType::Input));
        AddPin(CreatePin("Output", PinType::Output));
	}

    Ref<Node> MouseReleaseNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(playback != nullptr, "MouseReleaseNode: Playback system is null in MouseReleaseNode execution");
        
		playback->SimulateMouseButtonRelease(m_Button, m_X, m_Y);
        LUMINA_LOG_INFO("MouseReleaseNode: Simulated mouse release on {} button", Lumina::Input::MouseCodeToString(m_Button));
        
        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;

        return nullptr;
	}

    Lumina::MouseCode MouseReleaseNode::GetButton() const
    {
        return m_Button;
    }
}
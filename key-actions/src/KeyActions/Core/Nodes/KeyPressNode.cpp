#include "KeyPressNode.h"
#include <iostream>
#include "Lumina/Core/Log.h"
#include "Lumina/Core/Assert.h"
#include "Lumina/Core/Input.h"

namespace KeyActions
{
    KeyPressNode::KeyPressNode(Lumina::KeyCode key) : Node("KeyPress"), m_Key(key)
    {
        m_Name = "Press " + Lumina::Input::KeyCodeToString(m_Key);
        AddPin(CreatePin("Input", PinType::Input));
        AddPin(CreatePin("Output", PinType::Output));
    }

    Ref<Node> KeyPressNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(m_Key != Lumina::KeyCode::Unknown, "KeyPressNode: Invalid key code");
        LUMINA_ASSERT(playback != nullptr, "KeyPressNode: Playback system is null in KeyPressNode execution");

        playback->SimulateKeyPress(m_Key);
        LUMINA_LOG_INFO("Simulated key press of {}", Lumina::Input::KeyCodeToString(m_Key));

        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;

        return nullptr;
    }

    bool KeyPressNode::CanConnect(PinType sourceType, PinType targetType)
    {
        // Rule: KeyPressNode allows bidirectional connections
        // 1. Output pins can connect to Input pins
        // 2. Input pins can connect to Output pins
        return ((sourceType == PinType::Output && targetType == PinType::Input) ||
            (sourceType == PinType::Input && targetType == PinType::Output));
    }

    Lumina::KeyCode KeyPressNode::GetKey() const
    {
        return m_Key;
    }

    Lumina::Ref<KeyPressNode> KeyPressNode::Create(Lumina::KeyCode key)
    {
        return Lumina::CreateRef<KeyPressNode>(key);
    }
}
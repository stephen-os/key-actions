#pragma once

#include "Node.h"

#include "Lumina/Core/KeyCodes.h"

namespace KeyActions
{
    class KeyPressNode : public Node
    {
    public:
        static Ref<KeyPressNode> Create(Lumina::KeyCode key);

        KeyPressNode(Lumina::KeyCode key);

        Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) override;
        NodeType GetType() const override { return NodeType::KeyPress; }

        Lumina::KeyCode GetKey() const;

    private:
        Lumina::KeyCode m_Key;
    };
}
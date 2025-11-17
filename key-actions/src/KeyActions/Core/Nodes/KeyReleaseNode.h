#pragma once

#include "Node.h"
#include "Lumina/Core/KeyCodes.h"

namespace KeyActions
{
    class KeyReleaseNode : public Node
    {
    public:
        static Lumina::Ref<KeyReleaseNode> Create(Lumina::KeyCode key);

        KeyReleaseNode(Lumina::KeyCode key);

        Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) override;

        NodeType GetType() const override { return NodeType::KeyPress; }

        bool CanConnect(PinType sourceType, PinType targetType) override;

        Lumina::KeyCode GetKey() const;

    private:
        Lumina::KeyCode m_Key;
    };
}
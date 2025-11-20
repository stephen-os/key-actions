#pragma once

#include "Node.h"
#include "Lumina/Core/KeyCodes.h"

namespace KeyActions
{
    class KeyReleaseNode : public Node
    {
    public:
        static Ref<KeyReleaseNode> Create(Lumina::KeyCode key);

        KeyReleaseNode(Lumina::KeyCode key);

        Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) override;
        NodeType GetType() const override { return NodeType::KeyRelease; }

        Lumina::KeyCode GetKey() const;

    private:
        Lumina::KeyCode m_Key;
    };
}
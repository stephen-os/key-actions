#pragma once

#include "Node.h"

#include "Lumina/Core/KeyCodes.h"

namespace KeyActions
{
    class MouseReleaseNode : public Node
    {
    public:
        static Ref<MouseReleaseNode> Create(Lumina::MouseCode button, int x, int y);

        MouseReleaseNode(Lumina::MouseCode button, int x, int y);

        Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) override;
        NodeType GetType() const override { return NodeType::MouseRelease; }

        Lumina::MouseCode GetButton() const;
        int GetX() const;
        int GetY() const;

    private:
        Lumina::MouseCode m_Button;
        int m_X = 0;
        int m_Y = 0;
    };
}
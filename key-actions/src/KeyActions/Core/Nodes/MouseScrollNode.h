#pragma once

#include "Node.h"

namespace KeyActions
{
    class MouseScrollNode : public Node
    {
    public:
        static Ref<MouseScrollNode> Create(int scrollDX, int scrollDY);

        MouseScrollNode(int scrollDX, int scrollDY);

        Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) override;
        NodeType GetType() const override { return NodeType::MouseScroll; }

        int GetScrollDX() const;
        int GetScrollDY() const;

    private:
        int m_ScrollDX;
        int m_ScrollDY;
    };
}
#pragma once

#include "Node.h"

namespace KeyActions
{
    class MouseScrollNode : public Node
    {
    public:
        static Lumina::Ref<MouseScrollNode> Create(int scrollDX, int scrollDY);
        
        MouseScrollNode(int scrollDX, int scrollDY);

        Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) override;
		
        NodeType GetType() const override { return NodeType::MouseScroll; }
		
        bool CanConnect(PinType sourceType, PinType targetType) override;

        int GetScrollDX() const;
        int GetScrollDY() const;

    private:
        int m_ScrollDX;
        int m_ScrollDY;
    };
}
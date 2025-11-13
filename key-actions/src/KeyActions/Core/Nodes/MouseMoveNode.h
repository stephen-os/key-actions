#pragma once

#include "Node.h"

namespace KeyActions
{

    class MouseMoveNode : public Node
    {
    public:
        static Lumina::Ref<MouseMoveNode> Create(int x, int y);
        
        MouseMoveNode(int x, int y);

        NodePtr Execute(Lumina::GlobalInputPlayback* playback) override;

        NodeType GetType() const override { return NodeType::MouseMove; }

        bool CanConnect(PinType sourceType, PinType targetType) override;

        int GetX() const;
        int GetY() const;

    private:
        int m_X;
        int m_Y;
    };
}
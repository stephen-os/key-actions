#pragma once

#include "Node.h"

#include "Lumina/Core/KeyCodes.h"

namespace KeyActions
{
    class MousePressNode : public Node
    {
    public:
        static Lumina::Ref<MousePressNode> Create(Lumina::MouseCode button, int x, int y);
        
        MousePressNode(Lumina::MouseCode button, int x, int y);

        Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) override;

		NodeType GetType() const override { return NodeType::MousePress; }

        Lumina::MouseCode GetButton() const;

    private:
        Lumina::MouseCode m_Button;

		int m_X = 0;
		int m_Y = 0;
    };
}
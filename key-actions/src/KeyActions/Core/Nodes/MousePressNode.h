#pragma once

#include "Node.h"

#include "Lumina/Core/KeyCodes.h"

namespace KeyActions
{
    class MousePressNode : public Node
    {
    public:
        static Lumina::Ref<MousePressNode> Create(Lumina::MouseCode button);
        MousePressNode(Lumina::MouseCode button);

		NodePtr Execute(Lumina::GlobalInputPlayback* playback) override;

		NodeType GetType() const override { return NodeType::MousePress; }

		bool CanConnect(PinType sourceType, PinType targetType) override;

        Lumina::MouseCode GetButton() const;

        void SetModifiers(bool shift, bool ctrl, bool alt, bool super);

    private:
        Lumina::MouseCode m_Button;
    };
}
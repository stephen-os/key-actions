#pragma once
#include "Node.h"

namespace KeyActions
{
    class StartNode : public Node
    {
    public:
        static Lumina::Ref<StartNode> Create();

        StartNode();

        NodePtr Execute(Lumina::GlobalInputPlayback* playback) override;

		NodeType GetType() const override { return NodeType::Start; }

        bool CanConnect(PinType sourceType, PinType targetType) override;
    };
}
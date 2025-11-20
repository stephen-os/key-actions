#pragma once
#include "Node.h"

namespace KeyActions
{
    class StartNode : public Node
    {
    public:
        static Lumina::Ref<StartNode> Create();

        StartNode();

        Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) override;

		NodeType GetType() const override { return NodeType::Start; }
    };
}
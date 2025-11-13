#pragma once

#include "Node.h"

namespace KeyActions
{
    class EndNode : public Node
    {
    public:
        static Lumina::Ref<EndNode> Create();
        
        EndNode();

        NodePtr Execute(Lumina::GlobalInputPlayback* playback) override;
        NodeType GetType() const override { return NodeType::End; }
        bool CanConnect(PinType sourceType, PinType targetType) override;

        void SetExitCode(int code) { m_ExitCode = code; }
        int GetExitCode() const { return m_ExitCode; }

    private:
        int m_ExitCode = 0;
    };
}
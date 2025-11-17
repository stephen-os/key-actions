#include "StartNode.h"
#include "Lumina/Core/Log.h"
#include "Lumina/Core/Assert.h"

namespace KeyActions
{
    StartNode::StartNode() : Node("Start")
    {
        m_Name = "Start Node";
        AddPin(CreatePin("Output", PinType::Output));
    }

    Ref<Node> StartNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(playback != nullptr, "Playback system is null in StartNode execution");
        LUMINA_LOG_INFO("StartNode: Beginning execution");

        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;

        LUMINA_LOG_CRITICAL("StartNode: No connected nodes to execute");
        return nullptr;
    }

    bool StartNode::CanConnect(PinType sourceType, PinType targetType)
    {
        // Rule: StartNode only allows Output to Input connections
        return (sourceType == PinType::Output && targetType == PinType::Input);
    }

    Lumina::Ref<StartNode> StartNode::Create()
    {
        return Lumina::CreateRef<StartNode>();
    }
}
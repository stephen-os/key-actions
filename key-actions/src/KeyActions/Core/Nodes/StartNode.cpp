#include "StartNode.h"

#include "Lumina/Core/Log.h"
#include "Lumina/Core/Assert.h"

namespace KeyActions
{
    Ref<StartNode> StartNode::Create()
    {
        return Lumina::CreateRef<StartNode>();
    }

    StartNode::StartNode() : Node("Start Node")
    {
        AddPin(CreatePin("Output", PinType::Output));
    }

    Ref<Node> StartNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(playback != nullptr, "StartNode: Playback system is null in StartNode execution");
        LUMINA_LOG_INFO("StartNode: Beginning execution");

        Pin* outputPin = GetPin(PinType::Output);
        if (outputPin && outputPin->ConnectedNode)
            return outputPin->ConnectedNode;

        LUMINA_LOG_CRITICAL("StartNode: No connected nodes to execute");
        return nullptr;
    }
}
#include "EndNode.h"

#include "Lumina/Core/Log.h"
#include "Lumina/Core/Assert.h"

namespace KeyActions
{
    Lumina::Ref<EndNode> EndNode::Create()
    {
        return Lumina::CreateRef<EndNode>();
    }

    EndNode::EndNode() : Node("End")
    {
        m_Name = "End Node";
        AddPin(CreatePin("Input", PinType::Input));
    }

    Node::NodePtr EndNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(playback != nullptr, "Playback system is null in EndNode execution");
        LUMINA_LOG_INFO("EndNode: Finished execution with exit code {}", m_ExitCode);
        return nullptr;
    }

    bool EndNode::CanConnect(PinType sourceType, PinType targetType)
    {
        return (sourceType == PinType::Output && targetType == PinType::Input);
    }
}
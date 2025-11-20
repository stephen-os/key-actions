#include "EndNode.h"

#include "Lumina/Core/Log.h"
#include "Lumina/Core/Assert.h"

namespace KeyActions
{
    Ref<EndNode> EndNode::Create()
    {
        return Lumina::CreateRef<EndNode>();
    }

    EndNode::EndNode() : Node("End Node")
    {
        AddPin(CreatePin("Input", PinType::Input));
    }

    Ref<Node> EndNode::Execute(Lumina::GlobalInputPlayback* playback)
    {
        LUMINA_ASSERT(playback != nullptr, "Playback system is null in EndNode execution");
        LUMINA_LOG_INFO("EndNode: Finished execution with exit code {}", m_ExitCode);
        return nullptr;
    }
}
#include "NodeGraph.h"

#include <queue>
#include <unordered_set>
#include <algorithm>

namespace KeyActions
{
    Ref<Node> NodeGraph::AddNode(std::unique_ptr<Node> node)
    {
        LUMINA_ASSERT(node != nullptr, "NodeGraph::AddNode: Cannot add null node");

        NodeID nodeId = node->GetNodeID();
        LUMINA_ASSERT(!HasNode(nodeId), "NodeGraph::AddNode: Node with this ID already exists");

        Node* rawPtr = node.get();
        m_Nodes[nodeId] = std::move(node);

		// Non-owning Ref to the node
		return Ref<Node>(rawPtr, [](Node*) {});
    }

    bool NodeGraph::RemoveNode(const NodeID& nodeId)
    {
        auto it = m_Nodes.find(nodeId);
        if (it == m_Nodes.end())
            return false;

        // Disconnect all connections to/from this node
        DisconnectAllFromNode(nodeId);

        m_Nodes.erase(it);
        return true;
    }

    Ref<Node> NodeGraph::GetNode(const NodeID& nodeId) const
    {
        auto it = m_Nodes.find(nodeId);
        if (it == m_Nodes.end())
            return nullptr;

        // Non-owning Ref to the node
		return Ref<Node>(it->second.get(), [](Node*) {});
    }

    bool NodeGraph::HasNode(const NodeID& nodeId) const
    {
        return m_Nodes.find(nodeId) != m_Nodes.end();
    }

    const std::unordered_map<NodeID, std::unique_ptr<Node>>& NodeGraph::GetNodes() const
    {
        return m_Nodes;
    }

    size_t NodeGraph::GetNodeCount() const
    {
        return m_Nodes.size();
    }

    bool NodeGraph::IsEmpty() const
    {
        return m_Nodes.empty();
    }

    bool NodeGraph::ConnectPins(const NodeID& nodeAId, PinType pinAType, const NodeID& nodeBId, PinType pinBType)
    {
        Ref<Node> nodeA = GetNode(nodeAId);
        Ref<Node> nodeB = GetNode(nodeBId);

        if (!nodeA || !nodeB)
        {
            LUMINA_LOG_WARN("NodeGraph::ConnectPins: One or both nodes not found in graph");
            return false;
        }

        return Node::ConnectPins(nodeA, pinAType, nodeB, pinBType);
    }

    bool NodeGraph::DisconnectPin(const NodeID& nodeId, PinType pinType)
    {
        Ref<Node> node = GetNode(nodeId);
        if (!node)
        {
            LUMINA_LOG_WARN("NodeGraph::DisconnectPin: Node not found in graph");
            return false;
        }

        return Node::DisconnectPin(node, pinType);
    }

    void NodeGraph::DisconnectAllFromNode(const NodeID& nodeId)
    {
        Ref<Node> node = GetNode(nodeId);
        if (!node)
            return;

        // Disconnect all pins on this node
        for (auto& pin : node->GetPins())
        {
            if (pin.ConnectedNode && pin.LinkId.Get() != LINK_ID_NONE)
            {
                Node::DisconnectPin(node, pin.Type);
            }
        }
    }

    std::vector<LinkInfo> NodeGraph::GetAllConnections() const
    {
		// Collect all unique connections
        std::vector<LinkInfo> connections;

        // Track links we've already added
        std::unordered_set<uint64_t> seenLinks;

        for (const auto& [nodeId, node] : m_Nodes)
        {
            for (const auto& pin : node->GetPins())
            {
                if (pin.ConnectedNode && pin.LinkId.Get() != LINK_ID_NONE)
                {
                    // Only add each link once (it's bidirectional)
                    if (seenLinks.find(pin.LinkId.Get()) == seenLinks.end())
                    {
                        seenLinks.insert(pin.LinkId.Get());

                        // Find the connected pin
                        Node::Pin* connectedPin = nullptr;
                        for (auto& p : pin.ConnectedNode->GetPins())
                        {
                            if (p.LinkId == pin.LinkId)
                            {
                                connectedPin = &p;
                                break;
                            }
                        }

                        if (connectedPin)
                        {
                            LinkInfo info;
                            info.Id = pin.LinkId;
                            info.NodeAId = nodeId;
                            info.PinAType = pin.Type;
                            info.NodeBId = pin.ConnectedNode->GetNodeID();
                            info.PinBType = connectedPin->Type;
                            connections.push_back(info);
                        }
                    }
                }
            }
        }

        return connections;
    }

    std::vector<Ref<Node>> NodeGraph::GetConnectedNodes(const NodeID& nodeId) const
    {
        std::vector<Ref<Node>> connected;
        Ref<Node> node = GetNode(nodeId);

        if (!node)
            return connected;

		// Track seen node IDs to avoid duplicates
        std::unordered_set<uint64_t> seenNodeIds;

        for (const auto& pin : node->GetPins())
        {
            if (pin.ConnectedNode)
            {
                uint64_t connectedId = pin.ConnectedNode->GetNodeID().Get();
                if (seenNodeIds.find(connectedId) == seenNodeIds.end())
                {
                    seenNodeIds.insert(connectedId);
                    connected.push_back(pin.ConnectedNode);
                }
            }
        }

        return connected;
    }

    std::vector<ConnectionInfo> NodeGraph::GetIncomingConnections(const NodeID& nodeId) const
    {
        std::vector<ConnectionInfo> incoming;
        Ref<Node> node = GetNode(nodeId);

        if (!node)
            return incoming;

        for (const auto& pin : node->GetPins())
        {
            if (pin.Type == PinType::Input && pin.ConnectedNode)
            {
                // Find the connected pin on the other node
                for (const auto& remotePin : pin.ConnectedNode->GetPins())
                {
                    if (remotePin.LinkId == pin.LinkId)
                    {
                        ConnectionInfo info;
                        info.ConnectedNode = pin.ConnectedNode;
                        info.LocalPin = pin.Type;
                        info.RemotePin = remotePin.Type;
                        info.Link = pin.LinkId;
                        incoming.push_back(info);
                        break;
                    }
                }
            }
        }

        return incoming;
    }

    std::vector<ConnectionInfo> NodeGraph::GetOutgoingConnections(const NodeID& nodeId) const
    {
        std::vector<ConnectionInfo> outgoing;
        Ref<Node> node = GetNode(nodeId);

        if (!node)
            return outgoing;

        for (const auto& pin : node->GetPins())
        {
            if (pin.Type == PinType::Output && pin.ConnectedNode)
            {
                // Find the connected pin on the other node
                for (const auto& remotePin : pin.ConnectedNode->GetPins())
                {
                    if (remotePin.LinkId == pin.LinkId)
                    {
                        ConnectionInfo info;
                        info.ConnectedNode = pin.ConnectedNode;
                        info.LocalPin = pin.Type;
                        info.RemotePin = remotePin.Type;
                        info.Link = pin.LinkId;
                        outgoing.push_back(info);
                        break;
                    }
                }
            }
        }

        return outgoing;
    }

    bool NodeGraph::AreNodesConnected(const NodeID& nodeAId, const NodeID& nodeBId) const
    {
        Ref<Node> nodeA = GetNode(nodeAId);
        if (!nodeA)
            return false;

        for (const auto& pin : nodeA->GetPins())
        {
            if (pin.ConnectedNode && pin.ConnectedNode->GetNodeID() == nodeBId)
                return true;
        }

        return false;
    }

    bool NodeGraph::HasCycles() const
    {
        // Use DFS with three colors: white (unvisited), gray (visiting), black (visited)
        std::unordered_map<uint64_t, int> colors; // 0 = white, 1 = gray, 2 = black

        // Initialize all nodes as white
        for (const auto& [nodeId, node] : m_Nodes)
        {
            colors[nodeId.Get()] = 0;
        }

        // Helper function for DFS
        std::function<bool(uint64_t)> hasCycleDFS = [&](uint64_t nodeIdValue) -> bool
            {
                colors[nodeIdValue] = 1; // Mark as gray (visiting)

                Ref<Node> node = GetNode(NodeID(nodeIdValue));
                if (!node)
                    return false;

                // Visit all outgoing connections
                auto outgoing = GetOutgoingConnections(NodeID(nodeIdValue));
                for (const auto& conn : outgoing)
                {
                    uint64_t neighborId = conn.ConnectedNode->GetNodeID().Get();

                    if (colors[neighborId] == 1) // Gray = back edge = cycle!
                        return true;

                    if (colors[neighborId] == 0) // White = unvisited
                    {
                        if (hasCycleDFS(neighborId))
                            return true;
                    }
                }

                colors[nodeIdValue] = 2; // Mark as black (visited)
                return false;
            };

        // Check each component
        for (const auto& [nodeId, node] : m_Nodes)
        {
            if (colors[nodeId.Get()] == 0) // Unvisited
            {
                if (hasCycleDFS(nodeId.Get()))
                    return true;
            }
        }

        return false;
    }

    std::optional<std::vector<Ref<Node>>> NodeGraph::GetTopologicalSort() const
    {
        if (HasCycles())
            return std::nullopt;

        // Kahn's algorithm
        std::unordered_map<uint64_t, int> inDegree;

        // Calculate in-degrees
        for (const auto& [nodeId, node] : m_Nodes)
        {
            inDegree[nodeId.Get()] = 0;
        }

        for (const auto& [nodeId, node] : m_Nodes)
        {
            auto outgoing = GetOutgoingConnections(nodeId);
            for (const auto& conn : outgoing)
            {
                inDegree[conn.ConnectedNode->GetNodeID().Get()]++;
            }
        }

        // Queue nodes with in-degree 0
        std::queue<uint64_t> queue;
        for (const auto& [nodeIdValue, degree] : inDegree)
        {
            if (degree == 0)
                queue.push(nodeIdValue);
        }

        std::vector<Ref<Node>> sorted;

        while (!queue.empty())
        {
            uint64_t nodeIdValue = queue.front();
            queue.pop();

            Ref<Node> node = GetNode(NodeID(nodeIdValue));
            if (node)
                sorted.push_back(node);

            auto outgoing = GetOutgoingConnections(NodeID(nodeIdValue));
            for (const auto& conn : outgoing)
            {
                uint64_t neighborId = conn.ConnectedNode->GetNodeID().Get();
                inDegree[neighborId]--;

                if (inDegree[neighborId] == 0)
                    queue.push(neighborId);
            }
        }

        if (sorted.size() != m_Nodes.size())
            return std::nullopt; // Shouldn't happen since we checked for cycles

        return sorted;
    }

    std::optional<std::vector<Ref<Node>>> NodeGraph::FindPath(const NodeID& startId,
        const NodeID& endId) const
    {
        if (!HasNode(startId) || !HasNode(endId))
            return std::nullopt;

        // BFS
        std::queue<uint64_t> queue;
        std::unordered_set<uint64_t> visited;
        std::unordered_map<uint64_t, uint64_t> parent;

        uint64_t startValue = startId.Get();
        uint64_t endValue = endId.Get();

        queue.push(startValue);
        visited.insert(startValue);
        parent[startValue] = startValue; // Mark start as its own parent

        bool found = false;

        while (!queue.empty())
        {
            uint64_t current = queue.front();
            queue.pop();

            if (current == endValue)
            {
                found = true;
                break;
            }

            auto connected = GetConnectedNodes(NodeID(current));
            for (const auto& neighbor : connected)
            {
                uint64_t neighborId = neighbor->GetNodeID().Get();

                if (visited.find(neighborId) == visited.end())
                {
                    visited.insert(neighborId);
                    parent[neighborId] = current;
                    queue.push(neighborId);
                }
            }
        }

        if (!found)
            return std::nullopt;

        // Reconstruct path
        std::vector<Ref<Node>> path;
        uint64_t current = endValue;

        while (current != startValue)
        {
            path.push_back(GetNode(NodeID(current)));
            current = parent[current];
        }
        path.push_back(GetNode(startId));

        std::reverse(path.begin(), path.end());
        return path;
    }

    std::vector<Ref<Node>> NodeGraph::GetReachableNodes(const NodeID& startId) const
    {
        std::vector<Ref<Node>> reachable;

        if (!HasNode(startId))
            return reachable;

        std::unordered_set<uint64_t> visited;
        std::queue<uint64_t> queue;

        uint64_t startValue = startId.Get();
        queue.push(startValue);
        visited.insert(startValue);

        while (!queue.empty())
        {
            uint64_t current = queue.front();
            queue.pop();

            Ref<Node> node = GetNode(NodeID(current));
            if (node && current != startValue) // Don't include the start node itself
                reachable.push_back(node);

            auto connected = GetConnectedNodes(NodeID(current));
            for (const auto& neighbor : connected)
            {
                uint64_t neighborId = neighbor->GetNodeID().Get();

                if (visited.find(neighborId) == visited.end())
                {
                    visited.insert(neighborId);
                    queue.push(neighborId);
                }
            }
        }

        return reachable;
    }

    // ============================================================
    // Validation
    // ============================================================

    bool NodeGraph::ValidateIntegrity() const
    {
        bool isValid = true;

        for (const auto& [nodeId, node] : m_Nodes)
        {
            for (const auto& pin : node->GetPins())
            {
                if (pin.ConnectedNode)
                {
                    // Check 1: Connected node exists in graph
                    if (!HasNode(pin.ConnectedNode->GetNodeID()))
                    {
                        LUMINA_LOG_ERROR("Validation: Node {} has connection to node not in graph",
                            nodeId.Get());
                        isValid = false;
                        continue;
                    }

                    // Check 2: LinkId is valid
                    if (pin.LinkId.Get() == LINK_ID_NONE)
                    {
                        LUMINA_LOG_ERROR("Validation: Node {} has connected node but invalid link ID",
                            nodeId.Get());
                        isValid = false;
                        continue;
                    }

                    // Check 3: Connection is bidirectional
                    bool foundBackLink = false;
                    for (const auto& remotePin : pin.ConnectedNode->GetPins())
                    {
                        if (remotePin.LinkId == pin.LinkId &&
                            remotePin.ConnectedNode &&
                            remotePin.ConnectedNode->GetNodeID() == nodeId)
                        {
                            foundBackLink = true;
                            break;
                        }
                    }

                    if (!foundBackLink)
                    {
                        LUMINA_LOG_ERROR("Validation: Node {} has one-way connection (not bidirectional)",
                            nodeId.Get());
                        isValid = false;
                    }
                }
                else if (pin.LinkId.Get() != LINK_ID_NONE)
                {
                    // Check 4: If no connected node, LinkId should be NONE
                    LUMINA_LOG_ERROR("Validation: Node {} has link ID but no connected node",
                        nodeId.Get());
                    isValid = false;
                }
            }
        }

        if (isValid)
            LUMINA_LOG_INFO("Graph validation passed: All connections are valid and bidirectional");
        else
            LUMINA_LOG_ERROR("Graph validation failed: Integrity issues found");

        return isValid;
    }

    bool NodeGraph::WouldCreateCycle(const NodeID& nodeAId, const NodeID& nodeBId) const
    {
        // If we connect A -> B, would it create a cycle?
        // This happens if there's already a path from B to A

        if (!HasNode(nodeAId) || !HasNode(nodeBId))
            return false;

        if (nodeAId == nodeBId)
            return true; // Self-loop

        // Check if there's a path from B to A (using BFS)
        std::unordered_set<uint64_t> visited;
        std::queue<uint64_t> queue;

        uint64_t startValue = nodeBId.Get();
        uint64_t targetValue = nodeAId.Get();

        queue.push(startValue);
        visited.insert(startValue);

        while (!queue.empty())
        {
            uint64_t current = queue.front();
            queue.pop();

            if (current == targetValue)
                return true; // Found path from B to A, so A->B would create cycle

            auto outgoing = GetOutgoingConnections(NodeID(current));
            for (const auto& conn : outgoing)
            {
                uint64_t neighborId = conn.ConnectedNode->GetNodeID().Get();

                if (visited.find(neighborId) == visited.end())
                {
                    visited.insert(neighborId);
                    queue.push(neighborId);
                }
            }
        }

        return false; // No path from B to A, so A->B is safe
    }

    // ============================================================
    // Bulk Operations
    // ============================================================

    void NodeGraph::Clear()
    {
        LUMINA_LOG_INFO("Clearing graph with {} nodes", m_Nodes.size());
        m_Nodes.clear();
    }

    std::unique_ptr<NodeGraph> NodeGraph::Clone() const
    {
        LUMINA_LOG_WARN("NodeGraph::Clone() not yet implemented - requires Node cloning support");
        // TODO: Implement when Node has a Clone() method
        // This would need to:
        // 1. Clone all nodes
        // 2. Recreate all connections with new LinkIds
        // 3. Update all ConnectedNode references to point to cloned nodes
        return nullptr;
    }

} // namespace KeyActions
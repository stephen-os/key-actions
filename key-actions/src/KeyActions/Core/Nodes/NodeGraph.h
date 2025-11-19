#pragma once

#include <unordered_map>
#include <vector>
#include <optional>
#include <memory>

#include "KeyActions/Core/Memory.h"
#include "KeyActions/Core/Nodes/Node.h"

namespace KeyActions
{
    struct LinkInfo
    {
        LinkID Id;
        NodeID NodeAId;
        PinType PinAType;
        NodeID NodeBId;
        PinType PinBType;
    };

    struct ConnectionInfo
    {
        Ref<Node> ConnectedNode;
        PinType LocalPin;
        PinType RemotePin;
        LinkID Link;
    };

    class NodeGraph
    {
    public:
        NodeGraph() = default;
        ~NodeGraph() = default;
        NodeGraph(const NodeGraph&) = delete;
        NodeGraph& operator=(const NodeGraph&) = delete;
        NodeGraph(NodeGraph&&) noexcept = default;
        NodeGraph& operator=(NodeGraph&&) noexcept = default;

        Ref<Node> AddNode(std::unique_ptr<Node> node);
        bool RemoveNode(const NodeID& nodeId);
        void Clear();

        Ref<Node> GetNode(const NodeID& nodeId) const;
        bool HasNode(const NodeID& nodeId) const;
        const std::unordered_map<NodeID, std::unique_ptr<Node>>& GetNodes() const;
        size_t GetNodeCount() const;
        bool IsEmpty() const;

        bool ConnectPins(const NodeID& nodeAId, PinType pinAType, const NodeID& nodeBId, PinType pinBType);
        bool DisconnectPin(const NodeID& nodeId, PinType pinType);
        void DisconnectAllFromNode(const NodeID& nodeId);

        std::vector<LinkInfo> GetAllConnections() const;
        std::vector<Ref<Node>> GetConnectedNodes(const NodeID& nodeId) const;
        std::vector<ConnectionInfo> GetIncomingConnections(const NodeID& nodeId) const;
        std::vector<ConnectionInfo> GetOutgoingConnections(const NodeID& nodeId) const;
        bool AreNodesConnected(const NodeID& nodeAId, const NodeID& nodeBId) const;

        bool HasCycles() const;
        std::optional<std::vector<Ref<Node>>> GetTopologicalSort() const;
        std::optional<std::vector<Ref<Node>>> FindPath(const NodeID& startId, const NodeID& endId) const;
        std::vector<Ref<Node>> GetReachableNodes(const NodeID& startId) const;
        bool WouldCreateCycle(const NodeID& nodeAId, const NodeID& nodeBId) const;

        bool ValidateIntegrity() const;
        std::unique_ptr<NodeGraph> Clone() const;

        auto begin() const { return m_Nodes.begin(); }
        auto end() const { return m_Nodes.end(); }

    private:
        std::unordered_map<NodeID, std::unique_ptr<Node>> m_Nodes;
    };
}
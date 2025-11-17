#pragma once

#include <unordered_map>
#include <vector>
#include <optional>
#include <memory>

#include "KeyActions/Core/Memory.h"
#include "KeyActions/Core/Nodes/Node.h"

namespace KeyActions
{
    // Information about a link between two pins
    struct LinkInfo
    {
        LinkID Id;
        NodeID NodeAId;
        PinType PinAType;
        NodeID NodeBId;
        PinType PinBType;
    };

    // Information about a connection from one node's perspective
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

        // Prevent copying (unique ownership of nodes)
        NodeGraph(const NodeGraph&) = delete;
        NodeGraph& operator=(const NodeGraph&) = delete;

        // Allow moving
        NodeGraph(NodeGraph&&) noexcept = default;
        NodeGraph& operator=(NodeGraph&&) noexcept = default;

        // Add a node to the graph and return a reference for immediate use
        // Time: O(1) average
        Ref<Node> AddNode(std::unique_ptr<Node> node);

        // Remove a node and all its connections
        // Time: O(C) where C = number of connections to this node
        bool RemoveNode(const NodeID& nodeId);

        // Get a node by ID
        // Time: O(1) average
        Ref<Node> GetNode(const NodeID& nodeId) const;

        // Check if node exists
        // Time: O(1) average
        bool HasNode(const NodeID& nodeId) const;

        // Get all nodes (returns const reference to internal container)
        // Time: O(1)
        const std::unordered_map<NodeID, std::unique_ptr<Node>>& GetNodes() const;

        // Iterator support for range-based for loops
        // Time: O(1)
        auto begin() const { return m_Nodes.begin(); }
        auto end() const { return m_Nodes.end(); }

        // Get number of nodes
        // Time: O(1)
        size_t GetNodeCount() const;

        // Check if graph is empty
        // Time: O(1)
        bool IsEmpty() const;

        // Connect two pins (disconnects any existing connections on those pins first)
        // Time: O(P) where P = pins per node (for disconnect check)
        bool ConnectPins(const NodeID& nodeAId, PinType pinAType, const NodeID& nodeBId, PinType pinBType);

        // Disconnect a specific pin (also disconnects the connected pin on the other node)
        // Time: O(P) where P = number of pins on connected node
        bool DisconnectPin(const NodeID& nodeId, PinType pinType);

        // Disconnect all connections from a node
        // Time: O(C × P) where C = connections, P = pins per node
        void DisconnectAllFromNode(const NodeID& nodeId);

        // Get all connections (links) in the graph
        // Time: O(N × P) where N = nodes, P = pins per node
        std::vector<LinkInfo> GetAllConnections() const;

        // Get all nodes connected to a specific node
        // Time: O(P) where P = pins on the node
        std::vector<Ref<Node>> GetConnectedNodes(const NodeID& nodeId) const;

        // Get all incoming connections to a node
        // Time: O(P) where P = pins on the node
        std::vector<ConnectionInfo> GetIncomingConnections(const NodeID& nodeId) const;

        // Get all outgoing connections from a node
        // Time: O(P) where P = pins on the node
        std::vector<ConnectionInfo> GetOutgoingConnections(const NodeID& nodeId) const;

        // Check if two nodes are connected (via any pins)
        // Time: O(P) where P = pins on first node
        bool AreNodesConnected(const NodeID& nodeAId, const NodeID& nodeBId) const;

        // Check if graph contains cycles
        // Time: O(N + E) where N = nodes, E = edges
        bool HasCycles() const;

        // Get topological sort (returns nullopt if graph has cycles)
        // Time: O(N + E)
        std::optional<std::vector<Ref<Node>>> GetTopologicalSort() const;

        // Find a path between two nodes using BFS
        // Time: O(N + E)
        std::optional<std::vector<Ref<Node>>> FindPath(const NodeID& startId, const NodeID& endId) const;

        // Get all nodes reachable from a given node
        // Time: O(N + E)
        std::vector<Ref<Node>> GetReachableNodes(const NodeID& startId) const;

        // Validate graph integrity (all connections are bidirectional, no orphaned links, etc.)
        // Time: O(N × P)
        bool ValidateIntegrity() const;

        // Check if connecting two nodes would create a cycle
        // Time: O(N + E) worst case
        bool WouldCreateCycle(const NodeID& nodeAId, const NodeID& nodeBId) const;

        // Clear entire graph (removes all nodes and connections)
        // Time: O(N)
        void Clear();

        // Clone the entire graph (deep copy)
        // Time: O(N × P)
        std::unique_ptr<NodeGraph> Clone() const;

    private:
        std::unordered_map<NodeID, std::unique_ptr<Node>> m_Nodes;
    };
}
#include "NodeGraphTestSuite.h"

namespace KeyActions::Testing
{
    // ============================================================
    // Test Runner
    // ============================================================

    std::vector<TestResult> NodeGraphTestSuite::RunAllTests()
    {
        m_LastSummary = TestSummary();
        m_LastSummary.Results.clear();

        LUMINA_LOG_INFO("========================================");
        LUMINA_LOG_INFO("Running NodeGraph Test Suite");
        LUMINA_LOG_INFO("========================================");

        Lumina::Timer totalTimer;

        // Node Management Tests
        m_LastSummary.Results.push_back(RunTest("Add Node", [this]() { Test_AddNode(); }));
        m_LastSummary.Results.push_back(RunTest("Add Multiple Nodes", [this]() { Test_AddMultipleNodes(); }));
        m_LastSummary.Results.push_back(RunTest("Remove Node", [this]() { Test_RemoveNode(); }));
        m_LastSummary.Results.push_back(RunTest("Remove Nonexistent Node", [this]() { Test_RemoveNonexistentNode(); }));
        m_LastSummary.Results.push_back(RunTest("Get Node", [this]() { Test_GetNode(); }));
        m_LastSummary.Results.push_back(RunTest("Get Nonexistent Node", [this]() { Test_GetNonexistentNode(); }));
        m_LastSummary.Results.push_back(RunTest("Has Node", [this]() { Test_HasNode(); }));
        m_LastSummary.Results.push_back(RunTest("Get Node Count", [this]() { Test_GetNodeCount(); }));
        m_LastSummary.Results.push_back(RunTest("Is Empty", [this]() { Test_IsEmpty(); }));
        m_LastSummary.Results.push_back(RunTest("Clear", [this]() { Test_Clear(); }));

        // Connection Tests
        m_LastSummary.Results.push_back(RunTest("Connect Two Nodes", [this]() { Test_ConnectTwoNodes(); }));
        m_LastSummary.Results.push_back(RunTest("Connect Same Node Fails", [this]() { Test_ConnectSameNodeFails(); }));
        m_LastSummary.Results.push_back(RunTest("Connect Nonexistent Nodes Fails", [this]() { Test_ConnectNonexistentNodesFails(); }));
        m_LastSummary.Results.push_back(RunTest("Disconnect Pin", [this]() { Test_DisconnectPin(); }));
        m_LastSummary.Results.push_back(RunTest("Disconnect Nonexistent Pin", [this]() { Test_DisconnectNonexistentPin(); }));
        m_LastSummary.Results.push_back(RunTest("Reconnect Pin Disconnects Old", [this]() { Test_ReconnectPinDisconnectsOld(); }));
        m_LastSummary.Results.push_back(RunTest("Disconnect All From Node", [this]() { Test_DisconnectAllFromNode(); }));
        m_LastSummary.Results.push_back(RunTest("Get All Connections", [this]() { Test_GetAllConnections(); }));

        // Query Tests
        m_LastSummary.Results.push_back(RunTest("Get Connected Nodes", [this]() { Test_GetConnectedNodes(); }));
        m_LastSummary.Results.push_back(RunTest("Get Incoming Connections", [this]() { Test_GetIncomingConnections(); }));
        m_LastSummary.Results.push_back(RunTest("Get Outgoing Connections", [this]() { Test_GetOutgoingConnections(); }));
        m_LastSummary.Results.push_back(RunTest("Are Nodes Connected", [this]() { Test_AreNodesConnected(); }));
        m_LastSummary.Results.push_back(RunTest("Are Nodes Not Connected", [this]() { Test_AreNodesNotConnected(); }));

        // Graph Analysis Tests
        m_LastSummary.Results.push_back(RunTest("Has Cycles - No Cycle", [this]() { Test_HasCycles_NoCycle(); }));
        m_LastSummary.Results.push_back(RunTest("Has Cycles - With Cycle", [this]() { Test_HasCycles_WithCycle(); }));
        m_LastSummary.Results.push_back(RunTest("Topological Sort - Acyclic", [this]() { Test_TopologicalSort_Acyclic(); }));
        m_LastSummary.Results.push_back(RunTest("Topological Sort - Cyclic Returns Nullopt", [this]() { Test_TopologicalSort_CyclicReturnsNullopt(); }));
        m_LastSummary.Results.push_back(RunTest("Find Path - Exists", [this]() { Test_FindPath_Exists(); }));
        m_LastSummary.Results.push_back(RunTest("Find Path - Does Not Exist", [this]() { Test_FindPath_DoesNotExist(); }));
        m_LastSummary.Results.push_back(RunTest("Get Reachable Nodes", [this]() { Test_GetReachableNodes(); }));

        // Validation Tests
        m_LastSummary.Results.push_back(RunTest("Validate Integrity - Valid", [this]() { Test_ValidateIntegrity_Valid(); }));
        m_LastSummary.Results.push_back(RunTest("Would Create Cycle - True", [this]() { Test_WouldCreateCycle_True(); }));
        m_LastSummary.Results.push_back(RunTest("Would Create Cycle - False", [this]() { Test_WouldCreateCycle_False(); }));

        // Performance Tests
        m_LastSummary.Results.push_back(RunTest("Performance - Add Many Nodes", [this]() { Test_Performance_AddManyNodes(); }));
        m_LastSummary.Results.push_back(RunTest("Performance - Connect Many Nodes", [this]() { Test_Performance_ConnectManyNodes(); }));
        m_LastSummary.Results.push_back(RunTest("Performance - Iterate Nodes", [this]() { Test_Performance_IterateNodes(); }));

        m_LastSummary.TotalTimeMs = totalTimer.ElapsedMillis();

        // Calculate summary
        m_LastSummary.TotalTests = static_cast<int>(m_LastSummary.Results.size());
        for (const auto& result : m_LastSummary.Results)
        {
            if (result.Passed)
                m_LastSummary.PassedTests++;
            else
                m_LastSummary.FailedTests++;
        }

        LUMINA_LOG_INFO("========================================");
        LUMINA_LOG_INFO("Test Suite Complete");
        LUMINA_LOG_INFO("Total: {} | Passed: {} | Failed: {}",
            m_LastSummary.TotalTests,
            m_LastSummary.PassedTests,
            m_LastSummary.FailedTests);
        LUMINA_LOG_INFO("Total Time: {:.3f}ms", m_LastSummary.TotalTimeMs);
        LUMINA_LOG_INFO("========================================");

        return m_LastSummary.Results;
    }

    TestResult NodeGraphTestSuite::RunTest(const std::string& name, std::function<void()> testFunc)
    {
        TestResult result;
        result.TestName = name;
        result.Passed = false;

        Lumina::Timer timer;

        try
        {
            testFunc();
            result.Passed = true;
            result.Message = "Passed";
        }
        catch (const std::exception& e)
        {
            result.Passed = false;
            result.Message = std::string("Exception: ") + e.what();
        }
        catch (...)
        {
            result.Passed = false;
            result.Message = "Unknown exception";
        }

        result.ElapsedMs = timer.ElapsedMillis();

        if (result.Passed)
            LUMINA_LOG_INFO("[PASS] {} ({:.3f}ms)", name, result.ElapsedMs);
        else
            LUMINA_LOG_ERROR("[FAIL] {} - {} ({:.3f}ms)", name, result.Message, result.ElapsedMs);

        return result;
    }

    // ============================================================
    // Node Management Tests
    // ============================================================

    void NodeGraphTestSuite::Test_AddNode()
    {
        NodeGraph graph;
        auto node = std::make_unique<TestNode>("TestNode");
        auto nodeId = node->GetNodeID();

        auto nodeRef = graph.AddNode(std::move(node));

        if (!nodeRef)
            throw std::runtime_error("AddNode returned null reference");

        if (!graph.HasNode(nodeId))
            throw std::runtime_error("Node not found in graph after adding");

        if (graph.GetNodeCount() != 1)
            throw std::runtime_error("Node count is not 1 after adding one node");
    }

    void NodeGraphTestSuite::Test_AddMultipleNodes()
    {
        NodeGraph graph;

        for (int i = 0; i < 10; i++)
        {
            auto node = std::make_unique<TestNode>("TestNode" + std::to_string(i));
            graph.AddNode(std::move(node));
        }

        if (graph.GetNodeCount() != 10)
            throw std::runtime_error("Expected 10 nodes in graph");
    }

    void NodeGraphTestSuite::Test_RemoveNode()
    {
        NodeGraph graph;
        auto node = std::make_unique<TestNode>("TestNode");
        auto nodeId = node->GetNodeID();

        graph.AddNode(std::move(node));

        if (!graph.RemoveNode(nodeId))
            throw std::runtime_error("RemoveNode returned false");

        if (graph.HasNode(nodeId))
            throw std::runtime_error("Node still in graph after removal");

        if (graph.GetNodeCount() != 0)
            throw std::runtime_error("Node count is not 0 after removing all nodes");
    }

    void NodeGraphTestSuite::Test_RemoveNonexistentNode()
    {
        NodeGraph graph;
        NodeID fakeId(999999);

        if (graph.RemoveNode(fakeId))
            throw std::runtime_error("RemoveNode returned true for nonexistent node");
    }

    void NodeGraphTestSuite::Test_GetNode()
    {
        NodeGraph graph;
        auto node = std::make_unique<TestNode>("TestNode");
        auto nodeId = node->GetNodeID();

        graph.AddNode(std::move(node));

        auto retrieved = graph.GetNode(nodeId);
        if (!retrieved)
            throw std::runtime_error("GetNode returned null for existing node");

        if (retrieved->GetNodeID() != nodeId)
            throw std::runtime_error("Retrieved node has wrong ID");
    }

    void NodeGraphTestSuite::Test_GetNonexistentNode()
    {
        NodeGraph graph;
        NodeID fakeId(999999);

        auto retrieved = graph.GetNode(fakeId);
        if (retrieved)
            throw std::runtime_error("GetNode returned non-null for nonexistent node");
    }

    void NodeGraphTestSuite::Test_HasNode()
    {
        NodeGraph graph;
        auto node = std::make_unique<TestNode>("TestNode");
        auto nodeId = node->GetNodeID();

        if (graph.HasNode(nodeId))
            throw std::runtime_error("HasNode returned true before adding node");

        graph.AddNode(std::move(node));

        if (!graph.HasNode(nodeId))
            throw std::runtime_error("HasNode returned false after adding node");
    }

    void NodeGraphTestSuite::Test_GetNodeCount()
    {
        NodeGraph graph;

        if (graph.GetNodeCount() != 0)
            throw std::runtime_error("New graph has non-zero node count");

        for (int i = 0; i < 5; i++)
        {
            auto node = std::make_unique<TestNode>("Node" + std::to_string(i));
            graph.AddNode(std::move(node));
        }

        if (graph.GetNodeCount() != 5)
            throw std::runtime_error("Expected 5 nodes");
    }

    void NodeGraphTestSuite::Test_IsEmpty()
    {
        NodeGraph graph;

        if (!graph.IsEmpty())
            throw std::runtime_error("New graph is not empty");

        auto node = std::make_unique<TestNode>("TestNode");
        auto nodeId = node->GetNodeID();
        graph.AddNode(std::move(node));

        if (graph.IsEmpty())
            throw std::runtime_error("Graph is empty after adding node");

        graph.RemoveNode(nodeId);

        if (!graph.IsEmpty())
            throw std::runtime_error("Graph is not empty after removing all nodes");
    }

    void NodeGraphTestSuite::Test_Clear()
    {
        NodeGraph graph;

        for (int i = 0; i < 10; i++)
        {
            auto node = std::make_unique<TestNode>("Node" + std::to_string(i));
            graph.AddNode(std::move(node));
        }

        graph.Clear();

        if (!graph.IsEmpty())
            throw std::runtime_error("Graph not empty after Clear()");

        if (graph.GetNodeCount() != 0)
            throw std::runtime_error("Node count not 0 after Clear()");
    }

    // ============================================================
    // Connection Tests
    // ============================================================

    void NodeGraphTestSuite::Test_ConnectTwoNodes()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));

        if (!graph.ConnectPins(idA, PinType::Output, idB, PinType::Input))
            throw std::runtime_error("ConnectPins returned false");

        if (!graph.AreNodesConnected(idA, idB))
            throw std::runtime_error("Nodes not connected after ConnectPins");
    }

    void NodeGraphTestSuite::Test_ConnectSameNodeFails()
    {
        NodeGraph graph;
        auto node = std::make_unique<TestNode>("Node");
        auto id = node->GetNodeID();

        graph.AddNode(std::move(node));

        if (graph.ConnectPins(id, PinType::Output, id, PinType::Input))
            throw std::runtime_error("ConnectPins allowed self-connection");
    }

    void NodeGraphTestSuite::Test_ConnectNonexistentNodesFails()
    {
        NodeGraph graph;
        NodeID fakeId1(111111);
        NodeID fakeId2(222222);

        if (graph.ConnectPins(fakeId1, PinType::Output, fakeId2, PinType::Input))
            throw std::runtime_error("ConnectPins succeeded with nonexistent nodes");
    }

    void NodeGraphTestSuite::Test_DisconnectPin()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);

        if (!graph.DisconnectPin(idA, PinType::Output))
            throw std::runtime_error("DisconnectPin returned false");

        if (graph.AreNodesConnected(idA, idB))
            throw std::runtime_error("Nodes still connected after DisconnectPin");
    }

    void NodeGraphTestSuite::Test_DisconnectNonexistentPin()
    {
        NodeGraph graph;
        auto node = std::make_unique<TestNode>("Node");
        auto id = node->GetNodeID();

        graph.AddNode(std::move(node));

        // Disconnecting an unconnected pin should return false
        if (graph.DisconnectPin(id, PinType::Output))
            throw std::runtime_error("DisconnectPin returned true for unconnected pin");
    }

    void NodeGraphTestSuite::Test_ReconnectPinDisconnectsOld()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        // Connect A -> B
        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);

        // Connect A -> C (should disconnect A -> B)
        graph.ConnectPins(idA, PinType::Output, idC, PinType::Input);

        if (graph.AreNodesConnected(idA, idB))
            throw std::runtime_error("Old connection still exists");

        if (!graph.AreNodesConnected(idA, idC))
            throw std::runtime_error("New connection doesn't exist");
    }

    void NodeGraphTestSuite::Test_DisconnectAllFromNode()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idB, PinType::Output, idC, PinType::Input);

        graph.DisconnectAllFromNode(idB);

        if (graph.AreNodesConnected(idA, idB))
            throw std::runtime_error("Node A still connected to B");

        if (graph.AreNodesConnected(idB, idC))
            throw std::runtime_error("Node B still connected to C");
    }

    void NodeGraphTestSuite::Test_GetAllConnections()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idB, PinType::Output, idC, PinType::Input);

        auto connections = graph.GetAllConnections();

        if (connections.size() != 2)
            throw std::runtime_error("Expected 2 connections, got " + std::to_string(connections.size()));
    }

    // ============================================================
    // Query Tests
    // ============================================================

    void NodeGraphTestSuite::Test_GetConnectedNodes()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idA, PinType::Input, idC, PinType::Output);

        auto connected = graph.GetConnectedNodes(idA);

        if (connected.size() != 2)
            throw std::runtime_error("Expected 2 connected nodes, got " + std::to_string(connected.size()));
    }

    void NodeGraphTestSuite::Test_GetIncomingConnections()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);

        auto incoming = graph.GetIncomingConnections(idB);

        if (incoming.size() != 1)
            throw std::runtime_error("Expected 1 incoming connection");

        if (incoming[0].ConnectedNode->GetNodeID() != idA)
            throw std::runtime_error("Incoming connection from wrong node");
    }

    void NodeGraphTestSuite::Test_GetOutgoingConnections()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);

        auto outgoing = graph.GetOutgoingConnections(idA);

        if (outgoing.size() != 1)
            throw std::runtime_error("Expected 1 outgoing connection");

        if (outgoing[0].ConnectedNode->GetNodeID() != idB)
            throw std::runtime_error("Outgoing connection to wrong node");
    }

    void NodeGraphTestSuite::Test_AreNodesConnected()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);

        if (!graph.AreNodesConnected(idA, idB))
            throw std::runtime_error("AreNodesConnected returned false for connected nodes");
    }

    void NodeGraphTestSuite::Test_AreNodesNotConnected()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));

        if (graph.AreNodesConnected(idA, idB))
            throw std::runtime_error("AreNodesConnected returned true for disconnected nodes");
    }

    // ============================================================
    // Graph Analysis Tests
    // ============================================================

    void NodeGraphTestSuite::Test_HasCycles_NoCycle()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        // A -> B -> C (no cycle)
        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idB, PinType::Output, idC, PinType::Input);

        if (graph.HasCycles())
            throw std::runtime_error("HasCycles returned true for acyclic graph");
    }

    void NodeGraphTestSuite::Test_HasCycles_WithCycle()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        // A -> B -> C -> A (cycle)
        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idB, PinType::Output, idC, PinType::Input);
        graph.ConnectPins(idC, PinType::Output, idA, PinType::Input);

        if (!graph.HasCycles())
            throw std::runtime_error("HasCycles returned false for cyclic graph");
    }

    void NodeGraphTestSuite::Test_TopologicalSort_Acyclic()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idB, PinType::Output, idC, PinType::Input);

        auto sorted = graph.GetTopologicalSort();

        if (!sorted.has_value())
            throw std::runtime_error("Topological sort returned nullopt for acyclic graph");

        if (sorted->size() != 3)
            throw std::runtime_error("Topological sort wrong size");
    }

    void NodeGraphTestSuite::Test_TopologicalSort_CyclicReturnsNullopt()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));

        // Create cycle A <-> B
        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idB, PinType::Output, idA, PinType::Input);

        auto sorted = graph.GetTopologicalSort();

        if (sorted.has_value())
            throw std::runtime_error("Topological sort returned value for cyclic graph");
    }

    void NodeGraphTestSuite::Test_FindPath_Exists()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idB, PinType::Output, idC, PinType::Input);

        auto path = graph.FindPath(idA, idC);

        if (!path.has_value())
            throw std::runtime_error("FindPath returned nullopt for connected nodes");

        if (path->size() != 3)
            throw std::runtime_error("Path length incorrect");
    }

    void NodeGraphTestSuite::Test_FindPath_DoesNotExist()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));

        // No connection
        auto path = graph.FindPath(idA, idB);

        if (path.has_value())
            throw std::runtime_error("FindPath returned value for disconnected nodes");
    }

    void NodeGraphTestSuite::Test_GetReachableNodes()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");
        auto nodeD = std::make_unique<TestNode>("NodeD");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();
        auto idD = nodeD->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));
        graph.AddNode(std::move(nodeD));

        // A -> B -> C, D is isolated
        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idB, PinType::Output, idC, PinType::Input);

        auto reachable = graph.GetReachableNodes(idA);

        if (reachable.size() != 2) // B and C (not A itself)
            throw std::runtime_error("Expected 2 reachable nodes from A");
    }

    // ============================================================
    // Validation Tests
    // ============================================================

    void NodeGraphTestSuite::Test_ValidateIntegrity_Valid()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));

        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);

        if (!graph.ValidateIntegrity())
            throw std::runtime_error("ValidateIntegrity returned false for valid graph");
    }

    void NodeGraphTestSuite::Test_WouldCreateCycle_True()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        // A -> B -> C
        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);
        graph.ConnectPins(idB, PinType::Output, idC, PinType::Input);

        // Connecting C -> A would create cycle
        if (!graph.WouldCreateCycle(idC, idA))
            throw std::runtime_error("WouldCreateCycle returned false when it would create a cycle");
    }

    void NodeGraphTestSuite::Test_WouldCreateCycle_False()
    {
        NodeGraph graph;
        auto nodeA = std::make_unique<TestNode>("NodeA");
        auto nodeB = std::make_unique<TestNode>("NodeB");
        auto nodeC = std::make_unique<TestNode>("NodeC");

        auto idA = nodeA->GetNodeID();
        auto idB = nodeB->GetNodeID();
        auto idC = nodeC->GetNodeID();

        graph.AddNode(std::move(nodeA));
        graph.AddNode(std::move(nodeB));
        graph.AddNode(std::move(nodeC));

        // A -> B
        graph.ConnectPins(idA, PinType::Output, idB, PinType::Input);

        // Connecting B -> C would NOT create cycle
        if (graph.WouldCreateCycle(idB, idC))
            throw std::runtime_error("WouldCreateCycle returned true when it wouldn't create a cycle");
    }

    // ============================================================
    // Performance Tests
    // ============================================================

    void NodeGraphTestSuite::Test_Performance_AddManyNodes()
    {
        NodeGraph graph;
        const int COUNT = 1000;

        Lumina::Timer timer;
        for (int i = 0; i < COUNT; i++)
        {
            auto node = std::make_unique<TestNode>("Node" + std::to_string(i));
            graph.AddNode(std::move(node));
        }
        float elapsed = timer.ElapsedMillis();

        LUMINA_LOG_INFO("Added {} nodes in {:.3f}ms ({:.3f}μs per node)",
            COUNT, elapsed, (elapsed * 1000.0f) / COUNT);

        if (graph.GetNodeCount() != COUNT)
            throw std::runtime_error("Node count mismatch");
    }

    void NodeGraphTestSuite::Test_Performance_ConnectManyNodes()
    {
        NodeGraph graph;
        const int COUNT = 100;

        std::vector<NodeID> ids;
        for (int i = 0; i < COUNT; i++)
        {
            auto node = std::make_unique<TestNode>("Node" + std::to_string(i));
            ids.push_back(node->GetNodeID());
            graph.AddNode(std::move(node));
        }

        Lumina::Timer timer;
        // Create a chain: 0 -> 1 -> 2 -> ... -> 99
        for (int i = 0; i < COUNT - 1; i++)
        {
            graph.ConnectPins(ids[i], PinType::Output, ids[i + 1], PinType::Input);
        }
        float elapsed = timer.ElapsedMillis();

        LUMINA_LOG_INFO("Connected {} node pairs in {:.3f}ms ({:.3f}μs per connection)",
            COUNT - 1, elapsed, (elapsed * 1000.0f) / (COUNT - 1));
    }

    void NodeGraphTestSuite::Test_Performance_IterateNodes()
    {
        NodeGraph graph;
        const int COUNT = 10000;

        for (int i = 0; i < COUNT; i++)
        {
            auto node = std::make_unique<TestNode>("Node" + std::to_string(i));
            graph.AddNode(std::move(node));
        }

        Lumina::Timer timer;
        int count = 0;
        for (const auto& [id, node] : graph)
        {
            count++;
        }
        float elapsed = timer.ElapsedMillis();

        LUMINA_LOG_INFO("Iterated {} nodes in {:.3f}ms ({:.3f}μs per node)",
            COUNT, elapsed, (elapsed * 1000.0f) / COUNT);

        if (count != COUNT)
            throw std::runtime_error("Iteration count mismatch");
    }
}
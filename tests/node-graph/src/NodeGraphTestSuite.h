#pragma once

#include <string>
#include <vector>
#include <functional>
#include <sstream>

#include "KeyActions/Core/Nodes/NodeGraph.h"
#include "Lumina/Utils/Timer.h"

namespace KeyActions::Testing
{
    // Test result structure
    struct TestResult
    {
        std::string TestName;
        bool Passed;
        std::string Message;
        float ElapsedMs;
    };

    // Test suite for NodeGraph
    class NodeGraphTestSuite
    {
    public:
        NodeGraphTestSuite() = default;

        // Run all tests and return results
        std::vector<TestResult> RunAllTests();

        // Get summary statistics
        struct TestSummary
        {
            int TotalTests = 0;
            int PassedTests = 0;
            int FailedTests = 0;
            float TotalTimeMs = 0.0f;
            std::vector<TestResult> Results;
        };

        TestSummary GetLastSummary() const { return m_LastSummary; }

    private:
        TestSummary m_LastSummary;

        // Helper to run a single test
        TestResult RunTest(const std::string& name, std::function<void()> testFunc);

        // ============================================================
        // Node Management Tests
        // ============================================================
        void Test_AddNode();
        void Test_AddMultipleNodes();
        void Test_RemoveNode();
        void Test_RemoveNonexistentNode();
        void Test_GetNode();
        void Test_GetNonexistentNode();
        void Test_HasNode();
        void Test_GetNodeCount();
        void Test_IsEmpty();
        void Test_Clear();

        // ============================================================
        // Connection Tests
        // ============================================================
        void Test_ConnectTwoNodes();
        void Test_ConnectSameNodeFails();
        void Test_ConnectNonexistentNodesFails();
        void Test_DisconnectPin();
        void Test_DisconnectNonexistentPin();
        void Test_ReconnectPinDisconnectsOld();
        void Test_DisconnectAllFromNode();
        void Test_GetAllConnections();

        // ============================================================
        // Query Tests
        // ============================================================
        void Test_GetConnectedNodes();
        void Test_GetIncomingConnections();
        void Test_GetOutgoingConnections();
        void Test_AreNodesConnected();
        void Test_AreNodesNotConnected();

        // ============================================================
        // Graph Analysis Tests
        // ============================================================
        void Test_HasCycles_NoCycle();
        void Test_HasCycles_WithCycle();
        void Test_TopologicalSort_Acyclic();
        void Test_TopologicalSort_CyclicReturnsNullopt();
        void Test_FindPath_Exists();
        void Test_FindPath_DoesNotExist();
        void Test_GetReachableNodes();

        // ============================================================
        // Validation Tests
        // ============================================================
        void Test_ValidateIntegrity_Valid();
        void Test_WouldCreateCycle_True();
        void Test_WouldCreateCycle_False();

        // ============================================================
        // Performance Tests
        // ============================================================
        void Test_Performance_AddManyNodes();
        void Test_Performance_ConnectManyNodes();
        void Test_Performance_IterateNodes();

        // ============================================================
        // Helper Methods
        // ============================================================

        // Create a simple test node
        class TestNode : public Node
        {
        public:
            TestNode(const std::string& name) : Node(name)
            {
                // Add input and output pins
                AddPin(CreatePin("Input", PinType::Input));
                AddPin(CreatePin("Output", PinType::Output));
            }

            Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) override
            {
                // For testing, just return the connected output node
                Pin* outputPin = GetPin(PinType::Output);
                if (outputPin && outputPin->ConnectedNode)
                    return outputPin->ConnectedNode;
                return nullptr;
            }

            NodeType GetType() const override { return NodeType::KeyPress; }

            bool CanConnect(PinType sourceType, PinType targetType) override
            {
                // Output can connect to Input, Input can connect to Output
                return (sourceType == PinType::Output && targetType == PinType::Input) ||
                    (sourceType == PinType::Input && targetType == PinType::Output);
            }
        };
    };

} // namespace KeyActions::Testing
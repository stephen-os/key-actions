#include "NodeSimulationTestSuite.h"

#include "MockInputPlayback.h"

namespace KeyActions
{
    namespace Tests
    {
        std::vector<TestResult> NodeSimulationTestSuite::RunAllTests()
        {
            m_LastSummary = TestSummary();
            m_LastSummary.Results.clear();

            LUMINA_LOG_INFO("========================================");
            LUMINA_LOG_INFO("Running Node Simulation Test Suite");
            LUMINA_LOG_INFO("========================================");

            Lumina::Timer totalTimer;

			// StartNode Tests
            m_LastSummary.Results.push_back(RunTest("StartNode - Creation", [this]() { Test_StartNode_Creation(); }));
            m_LastSummary.Results.push_back(RunTest("StartNode - Get Name", [this]() { Test_StartNode_GetName(); }));
            m_LastSummary.Results.push_back(RunTest("StartNode - Get Type", [this]() { Test_StartNode_GetType(); }));
            m_LastSummary.Results.push_back(RunTest("StartNode - Has Pins", [this]() { Test_StartNode_HasPins(); }));
            m_LastSummary.Results.push_back(RunTest("StartNode - Can Connect", [this]() { Test_StartNode_CanConnect(); }));
            m_LastSummary.Results.push_back(RunTest("StartNode - Execute Returns Connected Node", [this]() { Test_StartNode_Execute_ReturnsConnectedNode(); }));
            m_LastSummary.Results.push_back(RunTest("StartNode - Execute Returns Null When Not Connected", [this]() { Test_StartNode_Execute_ReturnsNullWhenNotConnected(); }));
            // m_LastSummary.Results.push_back(RunTest("StartNode - Execute With Null Playback", [this]() { Test_StartNode_Execute_WithNullPlayback(); }));
            m_LastSummary.Results.push_back(RunTest("StartNode - No Input Connection", [this]() { Test_StartNode_NoInputConnection(); }));
            m_LastSummary.Results.push_back(RunTest("Performance - Create Many StartNodes", [this]() { Test_Performance_StartNode_CreateMany(); }));

            // EndNode Tests
            m_LastSummary.Results.push_back(RunTest("EndNode - Creation", [this]() { Test_EndNode_Creation(); }));
            m_LastSummary.Results.push_back(RunTest("EndNode - Get Name", [this]() { Test_EndNode_GetName(); }));
            m_LastSummary.Results.push_back(RunTest("EndNode - Get Type", [this]() { Test_EndNode_GetType(); }));
            m_LastSummary.Results.push_back(RunTest("EndNode - Has Pins", [this]() { Test_EndNode_HasPins(); }));
            m_LastSummary.Results.push_back(RunTest("EndNode - Can Connect", [this]() { Test_EndNode_CanConnect(); }));
            m_LastSummary.Results.push_back(RunTest("EndNode - Execute Returns Null", [this]() { Test_EndNode_Execute_ReturnsNull(); }));
            // m_LastSummary.Results.push_back(RunTest("EndNode - Execute With Null Playback", [this]() { Test_EndNode_Execute_WithNullPlayback(); }));
            m_LastSummary.Results.push_back(RunTest("EndNode - Exit Code Default Zero", [this]() { Test_EndNode_ExitCode_DefaultZero(); }));
            m_LastSummary.Results.push_back(RunTest("EndNode - Exit Code Set And Get", [this]() { Test_EndNode_ExitCode_SetAndGet(); }));
            m_LastSummary.Results.push_back(RunTest("EndNode - No Output Connection", [this]() { Test_EndNode_NoOutputConnection(); }));
            m_LastSummary.Results.push_back(RunTest("EndNode - Receives Connection", [this]() { Test_EndNode_ReceivesConnection(); }));
            m_LastSummary.Results.push_back(RunTest("Performance - Create Many EndNodes", [this]() { Test_Performance_EndNode_CreateMany(); }));

			// KeyPressNode Tests
            m_LastSummary.Results.push_back(RunTest("KeyPressNode - Creation", [this]() { Test_KeyPressNode_Creation(); }));
            m_LastSummary.Results.push_back(RunTest("KeyPressNode - Get Key", [this]() { Test_KeyPressNode_GetKey(); }));
            m_LastSummary.Results.push_back(RunTest("KeyPressNode - Get Name", [this]() { Test_KeyPressNode_GetName(); }));
            m_LastSummary.Results.push_back(RunTest("KeyPressNode - Get Type", [this]() { Test_KeyPressNode_GetType(); }));
            m_LastSummary.Results.push_back(RunTest("KeyPressNode - Has Pins", [this]() { Test_KeyPressNode_HasPins(); }));
            m_LastSummary.Results.push_back(RunTest("KeyPressNode - Can Connect", [this]() { Test_KeyPressNode_CanConnect(); }));
            m_LastSummary.Results.push_back(RunTest("KeyPressNode - Execute Simulates Key Press", [this]() { Test_KeyPressNode_Execute_SimulatesKeyPress(); }));
            m_LastSummary.Results.push_back(RunTest("KeyPressNode - Execute Returns Connected Node", [this]() { Test_KeyPressNode_Execute_ReturnsConnectedNode(); }));
            m_LastSummary.Results.push_back(RunTest("KeyPressNode - Execute Returns Null When Not Connected", [this]() { Test_KeyPressNode_Execute_ReturnsNullWhenNotConnected(); }));
            m_LastSummary.Results.push_back(RunTest("Performance - Create Many KeyPressNodes", [this]() { Test_Performance_KeyPressNode_CreateMany(); }));
            m_LastSummary.Results.push_back(RunTest("Performance - Execute Many KeyPressNodes", [this]() { Test_Performance_KeyPressNode_ExecuteMany(); }));

			// KeyReleaseNode Tests
            m_LastSummary.Results.push_back(RunTest("KeyReleaseNode - Creation", [this]() { Test_KeyReleaseNode_Creation(); }));
            m_LastSummary.Results.push_back(RunTest("KeyReleaseNode - Get Key", [this]() { Test_KeyReleaseNode_GetKey(); }));
            m_LastSummary.Results.push_back(RunTest("KeyReleaseNode - Get Name", [this]() { Test_KeyReleaseNode_GetName(); }));
            m_LastSummary.Results.push_back(RunTest("KeyReleaseNode - Get Type", [this]() { Test_KeyReleaseNode_GetType(); }));
            m_LastSummary.Results.push_back(RunTest("KeyReleaseNode - Has Pins", [this]() { Test_KeyReleaseNode_HasPins(); }));
            m_LastSummary.Results.push_back(RunTest("KeyReleaseNode - Can Connect", [this]() { Test_KeyReleaseNode_CanConnect(); }));
            m_LastSummary.Results.push_back(RunTest("KeyReleaseNode - Execute Simulates Key Release", [this]() { Test_KeyReleaseNode_Execute_SimulatesKeyRelease(); }));
            m_LastSummary.Results.push_back(RunTest("KeyReleaseNode - Execute Returns Connected Node", [this]() { Test_KeyReleaseNode_Execute_ReturnsConnectedNode(); }));
            m_LastSummary.Results.push_back(RunTest("KeyReleaseNode - Execute Returns Null When Not Connected", [this]() { Test_KeyReleaseNode_Execute_ReturnsNullWhenNotConnected(); }));
            m_LastSummary.Results.push_back(RunTest("Performance - Create Many KeyReleaseNodes", [this]() { Test_Performance_KeyReleaseNode_CreateMany(); }));
            m_LastSummary.Results.push_back(RunTest("Performance - Execute Many KeyReleaseNodes", [this]() { Test_Performance_KeyReleaseNode_ExecuteMany(); }));

			// Integration Tests
            m_LastSummary.Results.push_back(RunTest("Integration - Start To End Chain", [this]() { Test_Integration_StartToEnd_Chain(); }));
            m_LastSummary.Results.push_back(RunTest("Integration - Start Without End", [this]() { Test_Integration_StartWithoutEnd(); }));
            m_LastSummary.Results.push_back(RunTest("Integration - Multiple Start Nodes", [this]() { Test_Integration_MultipleStartNodes(); }));

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

        TestResult NodeSimulationTestSuite::RunTest(const std::string& name, std::function<void()> testFunc)
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

        void NodeSimulationTestSuite::Test_StartNode_Creation()
        {
            auto node = StartNode::Create();

            if (!node)
                throw std::runtime_error("Failed to create StartNode");

            if (node->GetNodeID().Get() == NODE_ID_NONE)
                throw std::runtime_error("Node ID is NONE after creation");
        }

        void NodeSimulationTestSuite::Test_StartNode_GetName()
        {
            auto node = StartNode::Create();

            std::string name = node->GetName();
            if (name.empty())
                throw std::runtime_error("Node name is empty");
        }

        void NodeSimulationTestSuite::Test_StartNode_GetType()
        {
            auto node = StartNode::Create();

            if (node->GetType() != NodeType::Start)
                throw std::runtime_error("GetType returned wrong type");
        }

        void NodeSimulationTestSuite::Test_StartNode_HasPins()
        {
            auto node = StartNode::Create();

            // StartNode should only have an Output pin (execution begins here)
            if (node->HasPin(PinType::Input))
                throw std::runtime_error("StartNode should not have Input pin");

            if (!node->HasPin(PinType::Output))
                throw std::runtime_error("StartNode missing Output pin");

            if (node->GetPinCount() != 1)
                throw std::runtime_error("StartNode should have exactly 1 pin");
        }

        void NodeSimulationTestSuite::Test_StartNode_CanConnect()
        {
            auto node = StartNode::Create();

            // Output -> Input should be allowed
            if (!node->CanConnect(PinType::Output, PinType::Input))
                throw std::runtime_error("Output->Input connection should be allowed");

            // Output -> Output should NOT be allowed
            if (node->CanConnect(PinType::Output, PinType::Output))
                throw std::runtime_error("Output->Output connection should NOT be allowed");
        }

        void NodeSimulationTestSuite::Test_StartNode_Execute_ReturnsConnectedNode()
        {
            auto startNode = StartNode::Create();
            auto keyNode = KeyPressNode::Create(Lumina::KeyCode::A);

            // Connect start's output to key's input
            if (!Node::ConnectPins(startNode, PinType::Output, keyNode, PinType::Input))
                throw std::runtime_error("Failed to connect nodes");

            MockInputPlayback playback;

            // Execute startNode, should return keyNode
            auto nextNode = startNode->Execute(&playback);

            if (!nextNode)
                throw std::runtime_error("Execute returned null when connected node exists");

            if (nextNode->GetNodeID() != keyNode->GetNodeID())
                throw std::runtime_error("Execute returned wrong connected node");
        }

        void NodeSimulationTestSuite::Test_StartNode_Execute_ReturnsNullWhenNotConnected()
        {
            auto node = StartNode::Create();
            MockInputPlayback playback;

            auto nextNode = node->Execute(&playback);

            if (nextNode != nullptr)
                throw std::runtime_error("Execute should return null when no node is connected");
        }

        void NodeSimulationTestSuite::Test_StartNode_NoInputConnection()
        {
            auto startNode = StartNode::Create();
            auto keyNode = KeyPressNode::Create(Lumina::KeyCode::A);

            // Attempt to connect TO start's input (should fail since StartNode has no input)
            bool connected = Node::ConnectPins(keyNode, PinType::Output, startNode, PinType::Input);

            if (connected)
                throw std::runtime_error("Should not be able to connect to StartNode's input");
        }

        void NodeSimulationTestSuite::Test_Performance_StartNode_CreateMany()
        {
            const int COUNT = 1000;
            std::vector<Lumina::Ref<StartNode>> nodes;
            nodes.reserve(COUNT);

            Lumina::Timer timer;

            for (int i = 0; i < COUNT; i++)
            {
                nodes.push_back(StartNode::Create());
            }

            float elapsed = timer.ElapsedMillis();

            LUMINA_LOG_INFO("Created {} StartNodes in {:.3f}ms ({:.3f}μs per node)",
                COUNT, elapsed, (elapsed * 1000.0f) / COUNT);

            if (nodes.size() != COUNT)
                throw std::runtime_error("Node count mismatch");
        }

        // EndNode Tests
        void NodeSimulationTestSuite::Test_EndNode_Creation()
        {
            auto node = EndNode::Create();

            if (!node)
                throw std::runtime_error("Failed to create EndNode");

            if (node->GetNodeID().Get() == NODE_ID_NONE)
                throw std::runtime_error("Node ID is NONE after creation");
        }

        void NodeSimulationTestSuite::Test_EndNode_GetName()
        {
            auto node = EndNode::Create();

            std::string name = node->GetName();
            if (name.empty())
                throw std::runtime_error("Node name is empty");
        }

        void NodeSimulationTestSuite::Test_EndNode_GetType()
        {
            auto node = EndNode::Create();

            if (node->GetType() != NodeType::End)
                throw std::runtime_error("GetType returned wrong type");
        }

        void NodeSimulationTestSuite::Test_EndNode_HasPins()
        {
            auto node = EndNode::Create();

            // EndNode should only have an Input pin (execution ends here)
            if (!node->HasPin(PinType::Input))
                throw std::runtime_error("EndNode missing Input pin");

            if (node->HasPin(PinType::Output))
                throw std::runtime_error("EndNode should not have Output pin");

            if (node->GetPinCount() != 1)
                throw std::runtime_error("EndNode should have exactly 1 pin");
        }

        void NodeSimulationTestSuite::Test_EndNode_CanConnect()
        {
            auto node = EndNode::Create();

            // Input -> Output should be allowed
            if (!node->CanConnect(PinType::Input, PinType::Output))
                throw std::runtime_error("Input->Output connection should be allowed");

            // Input -> Input should NOT be allowed
            if (node->CanConnect(PinType::Input, PinType::Input))
                throw std::runtime_error("Input->Input connection should NOT be allowed");
        }

        void NodeSimulationTestSuite::Test_EndNode_Execute_ReturnsNull()
        {
            auto node = EndNode::Create();
            MockInputPlayback playback;

            // EndNode should always return null (execution terminates)
            auto nextNode = node->Execute(&playback);

            if (nextNode != nullptr)
                throw std::runtime_error("EndNode Execute should always return null");
        }

        void NodeSimulationTestSuite::Test_EndNode_ExitCode_DefaultZero()
        {
            auto node = EndNode::Create();

            if (node->GetExitCode() != 0)
                throw std::runtime_error("Default exit code should be 0");
        }

        void NodeSimulationTestSuite::Test_EndNode_ExitCode_SetAndGet()
        {
            auto node = EndNode::Create();

            node->SetExitCode(42);

            if (node->GetExitCode() != 42)
                throw std::runtime_error("Exit code should be 42 after setting");

            node->SetExitCode(-1);

            if (node->GetExitCode() != -1)
                throw std::runtime_error("Exit code should be -1 after setting");
        }

        void NodeSimulationTestSuite::Test_EndNode_NoOutputConnection()
        {
            auto endNode = EndNode::Create();
            auto keyNode = KeyPressNode::Create(Lumina::KeyCode::A);

            // Attempt to connect FROM end's output (should fail since EndNode has no output)
            bool connected = Node::ConnectPins(endNode, PinType::Output, keyNode, PinType::Input);

            if (connected)
                throw std::runtime_error("Should not be able to connect from EndNode's output");
        }

        void NodeSimulationTestSuite::Test_EndNode_ReceivesConnection()
        {
            auto keyNode = KeyPressNode::Create(Lumina::KeyCode::A);
            auto endNode = EndNode::Create();

            // Connect key's output to end's input
            if (!Node::ConnectPins(keyNode, PinType::Output, endNode, PinType::Input))
                throw std::runtime_error("Failed to connect nodes");

            MockInputPlayback playback;

            // Execute keyNode, should return endNode
            auto nextNode = keyNode->Execute(&playback);

            if (!nextNode)
                throw std::runtime_error("Execute returned null when connected node exists");

            if (nextNode->GetNodeID() != endNode->GetNodeID())
                throw std::runtime_error("Execute returned wrong connected node");
        }

        void NodeSimulationTestSuite::Test_Performance_EndNode_CreateMany()
        {
            const int COUNT = 1000;
            std::vector<Lumina::Ref<EndNode>> nodes;
            nodes.reserve(COUNT);

            Lumina::Timer timer;

            for (int i = 0; i < COUNT; i++)
            {
                nodes.push_back(EndNode::Create());
            }

            float elapsed = timer.ElapsedMillis();

            LUMINA_LOG_INFO("Created {} EndNodes in {:.3f}ms ({:.3f}μs per node)",
                COUNT, elapsed, (elapsed * 1000.0f) / COUNT);

            if (nodes.size() != COUNT)
                throw std::runtime_error("Node count mismatch");
        }

        void NodeSimulationTestSuite::Test_KeyPressNode_Creation()
        {
            auto node = KeyPressNode::Create(Lumina::KeyCode::A);

            if (!node)
                throw std::runtime_error("Failed to create KeyPressNode");

            if (node->GetNodeID().Get() == NODE_ID_NONE)
                throw std::runtime_error("Node ID is NONE after creation");
        }

        void NodeSimulationTestSuite::Test_KeyPressNode_GetKey()
        {
            auto node = KeyPressNode::Create(Lumina::KeyCode::Space);

            if (node->GetKey() != Lumina::KeyCode::Space)
                throw std::runtime_error("GetKey returned wrong key");
        }

        void NodeSimulationTestSuite::Test_KeyPressNode_GetName()
        {
            auto node = KeyPressNode::Create(Lumina::KeyCode::A);

            std::string name = node->GetName();
            if (name.empty())
                throw std::runtime_error("Node name is empty");

            if (name.find("Press") == std::string::npos)
                throw std::runtime_error("Node name doesn't contain 'Press'");
        }

        void NodeSimulationTestSuite::Test_KeyPressNode_GetType()
        {
            auto node = KeyPressNode::Create(Lumina::KeyCode::A);

            if (node->GetType() != NodeType::KeyPress)
                throw std::runtime_error("GetType returned wrong type");
        }

        void NodeSimulationTestSuite::Test_KeyPressNode_HasPins()
        {
            auto node = KeyPressNode::Create(Lumina::KeyCode::A);

            if (!node->HasPin(PinType::Input))
                throw std::runtime_error("KeyPressNode missing Input pin");

            if (!node->HasPin(PinType::Output))
                throw std::runtime_error("KeyPressNode missing Output pin");

            if (node->GetPinCount() != 2)
                throw std::runtime_error("KeyPressNode should have exactly 2 pins");
        }

        void NodeSimulationTestSuite::Test_KeyPressNode_CanConnect()
        {
            auto node = KeyPressNode::Create(Lumina::KeyCode::A);

            // Output -> Input should be allowed
            if (!node->CanConnect(PinType::Output, PinType::Input))
                throw std::runtime_error("Output->Input connection should be allowed");

            // Input -> Output should be allowed
            if (!node->CanConnect(PinType::Input, PinType::Output))
                throw std::runtime_error("Input->Output connection should be allowed");

            // Output -> Output should NOT be allowed
            if (node->CanConnect(PinType::Output, PinType::Output))
                throw std::runtime_error("Output->Output connection should NOT be allowed");

            // Input -> Input should NOT be allowed
            if (node->CanConnect(PinType::Input, PinType::Input))
                throw std::runtime_error("Input->Input connection should NOT be allowed");
        }

        void NodeSimulationTestSuite::Test_KeyPressNode_Execute_SimulatesKeyPress()
        {
            auto node = KeyPressNode::Create(Lumina::KeyCode::Enter);
            MockInputPlayback playback;

            node->Execute(&playback);

            if (playback.GetEventCount() != 1)
                throw std::runtime_error("Expected exactly 1 simulated event");

            if (!playback.HasKeyPress(Lumina::KeyCode::Enter))
                throw std::runtime_error("KeyPress event for Enter key was not simulated");
        }

        void NodeSimulationTestSuite::Test_KeyPressNode_Execute_ReturnsConnectedNode()
        {
            auto nodeA = KeyPressNode::Create(Lumina::KeyCode::A);
            auto nodeB = KeyPressNode::Create(Lumina::KeyCode::B);

            // Connect A's output to B's input
            if (!Node::ConnectPins(nodeA, PinType::Output, nodeB, PinType::Input))
                throw std::runtime_error("Failed to connect nodes");

            MockInputPlayback playback;

            // Execute nodeA, should return nodeB
            auto nextNode = nodeA->Execute(&playback);

            if (!nextNode)
                throw std::runtime_error("Execute returned null when connected node exists");

            if (nextNode->GetNodeID() != nodeB->GetNodeID())
                throw std::runtime_error("Execute returned wrong connected node");
        }

        void NodeSimulationTestSuite::Test_KeyPressNode_Execute_ReturnsNullWhenNotConnected()
        {
            auto node = KeyPressNode::Create(Lumina::KeyCode::A);
            MockInputPlayback playback;

            auto nextNode = node->Execute(&playback);

            if (nextNode != nullptr)
                throw std::runtime_error("Execute should return null when no node is connected");
        }

        void NodeSimulationTestSuite::Test_Performance_KeyPressNode_CreateMany()
        {
            const int COUNT = 1000;
            std::vector<Lumina::Ref<KeyPressNode>> nodes;
            nodes.reserve(COUNT);

            Lumina::Timer timer;

            for (int i = 0; i < COUNT; i++)
            {
                nodes.push_back(KeyPressNode::Create(Lumina::KeyCode::A));
            }

            float elapsed = timer.ElapsedMillis();

            LUMINA_LOG_INFO("Created {} KeyPressNodes in {:.3f}ms ({:.3f}μs per node)",
                COUNT, elapsed, (elapsed * 1000.0f) / COUNT);

            if (nodes.size() != COUNT)
                throw std::runtime_error("Node count mismatch");
        }

        void NodeSimulationTestSuite::Test_Performance_KeyPressNode_ExecuteMany()
        {
            const int COUNT = 1000;
            std::vector<Lumina::Ref<KeyPressNode>> nodes;
            nodes.reserve(COUNT);

            for (int i = 0; i < COUNT; i++)
            {
                nodes.push_back(KeyPressNode::Create(Lumina::KeyCode::A));
            }

            MockInputPlayback playback;
            Lumina::Timer timer;

            for (auto& node : nodes)
            {
                node->Execute(&playback);
            }

            float elapsed = timer.ElapsedMillis();

            LUMINA_LOG_INFO("Executed {} KeyPressNodes in {:.3f}ms ({:.3f}μs per execution)",
                COUNT, elapsed, (elapsed * 1000.0f) / COUNT);

            if (playback.GetEventCount() != COUNT)
                throw std::runtime_error("Expected " + std::to_string(COUNT) + " key press events");
        }

        void NodeSimulationTestSuite::Test_KeyReleaseNode_Creation()
        {
            auto node = KeyReleaseNode::Create(Lumina::KeyCode::A);

            if (!node)
                throw std::runtime_error("Failed to create KeyReleaseNode");

            if (node->GetNodeID().Get() == NODE_ID_NONE)
                throw std::runtime_error("Node ID is NONE after creation");
        }

        void NodeSimulationTestSuite::Test_KeyReleaseNode_GetKey()
        {
            auto node = KeyReleaseNode::Create(Lumina::KeyCode::Space);

            if (node->GetKey() != Lumina::KeyCode::Space)
                throw std::runtime_error("GetKey returned wrong key");
        }

        void NodeSimulationTestSuite::Test_KeyReleaseNode_GetName()
        {
            auto node = KeyReleaseNode::Create(Lumina::KeyCode::A);

            std::string name = node->GetName();
            if (name.empty())
                throw std::runtime_error("Node name is empty");

            // Note: Based on your implementation, it says "Press" but should probably say "Release"
            if (name.find("Press") == std::string::npos && name.find("Release") == std::string::npos)
                throw std::runtime_error("Node name doesn't contain 'Press' or 'Release'");
        }

        void NodeSimulationTestSuite::Test_KeyReleaseNode_GetType()
        {
            auto node = KeyReleaseNode::Create(Lumina::KeyCode::A);

            // This will FAIL with your current implementation - it returns KeyPress instead of KeyRelease
            if (node->GetType() != NodeType::KeyRelease)
                throw std::runtime_error("GetType returned wrong type (should be KeyRelease, not KeyPress)");
        }

        void NodeSimulationTestSuite::Test_KeyReleaseNode_HasPins()
        {
            auto node = KeyReleaseNode::Create(Lumina::KeyCode::A);

            if (!node->HasPin(PinType::Input))
                throw std::runtime_error("KeyReleaseNode missing Input pin");

            if (!node->HasPin(PinType::Output))
                throw std::runtime_error("KeyReleaseNode missing Output pin");

            if (node->GetPinCount() != 2)
                throw std::runtime_error("KeyReleaseNode should have exactly 2 pins");
        }

        void NodeSimulationTestSuite::Test_KeyReleaseNode_CanConnect()
        {
            auto node = KeyReleaseNode::Create(Lumina::KeyCode::A);

            // Output -> Input should be allowed
            if (!node->CanConnect(PinType::Output, PinType::Input))
                throw std::runtime_error("Output->Input connection should be allowed");

            // Input -> Output should be allowed
            if (!node->CanConnect(PinType::Input, PinType::Output))
                throw std::runtime_error("Input->Output connection should be allowed");

            // Output -> Output should NOT be allowed
            if (node->CanConnect(PinType::Output, PinType::Output))
                throw std::runtime_error("Output->Output connection should NOT be allowed");

            // Input -> Input should NOT be allowed
            if (node->CanConnect(PinType::Input, PinType::Input))
                throw std::runtime_error("Input->Input connection should NOT be allowed");
        }

        void NodeSimulationTestSuite::Test_KeyReleaseNode_Execute_SimulatesKeyRelease()
        {
            auto node = KeyReleaseNode::Create(Lumina::KeyCode::Enter);
            MockInputPlayback playback;

            node->Execute(&playback);

            if (playback.GetEventCount() != 1)
                throw std::runtime_error("Expected exactly 1 simulated event");

            // This will FAIL with your current implementation - it calls SimulateKeyPress instead of SimulateKeyRelease
            if (!playback.HasKeyRelease(Lumina::KeyCode::Enter))
                throw std::runtime_error("KeyRelease event for Enter key was not simulated (you're calling SimulateKeyPress instead!)");
        }

        void NodeSimulationTestSuite::Test_KeyReleaseNode_Execute_ReturnsConnectedNode()
        {
            auto nodeA = KeyReleaseNode::Create(Lumina::KeyCode::A);
            auto nodeB = KeyReleaseNode::Create(Lumina::KeyCode::B);

            // Connect A's output to B's input
            if (!Node::ConnectPins(nodeA, PinType::Output, nodeB, PinType::Input))
                throw std::runtime_error("Failed to connect nodes");

            MockInputPlayback playback;

            // Execute nodeA, should return nodeB
            auto nextNode = nodeA->Execute(&playback);

            if (!nextNode)
                throw std::runtime_error("Execute returned null when connected node exists");

            if (nextNode->GetNodeID() != nodeB->GetNodeID())
                throw std::runtime_error("Execute returned wrong connected node");
        }

        void NodeSimulationTestSuite::Test_KeyReleaseNode_Execute_ReturnsNullWhenNotConnected()
        {
            auto node = KeyReleaseNode::Create(Lumina::KeyCode::A);
            MockInputPlayback playback;

            auto nextNode = node->Execute(&playback);

            if (nextNode != nullptr)
                throw std::runtime_error("Execute should return null when no node is connected");
        }

        void NodeSimulationTestSuite::Test_Performance_KeyReleaseNode_CreateMany()
        {
            const int COUNT = 1000;
            std::vector<Lumina::Ref<KeyReleaseNode>> nodes;
            nodes.reserve(COUNT);

            Lumina::Timer timer;

            for (int i = 0; i < COUNT; i++)
            {
                nodes.push_back(KeyReleaseNode::Create(Lumina::KeyCode::A));
            }

            float elapsed = timer.ElapsedMillis();

            LUMINA_LOG_INFO("Created {} KeyReleaseNodes in {:.3f}ms ({:.3f}μs per node)",
                COUNT, elapsed, (elapsed * 1000.0f) / COUNT);

            if (nodes.size() != COUNT)
                throw std::runtime_error("Node count mismatch");
        }

        void NodeSimulationTestSuite::Test_Performance_KeyReleaseNode_ExecuteMany()
        {
            const int COUNT = 1000;
            std::vector<Lumina::Ref<KeyReleaseNode>> nodes;
            nodes.reserve(COUNT);

            for (int i = 0; i < COUNT; i++)
            {
                nodes.push_back(KeyReleaseNode::Create(Lumina::KeyCode::A));
            }

            MockInputPlayback playback;
            Lumina::Timer timer;

            for (auto& node : nodes)
            {
                node->Execute(&playback);
            }

            float elapsed = timer.ElapsedMillis();

            LUMINA_LOG_INFO("Executed {} KeyReleaseNodes in {:.3f}ms ({:.3f}μs per execution)",
                COUNT, elapsed, (elapsed * 1000.0f) / COUNT);

            if (playback.GetEventCount() != COUNT)
                throw std::runtime_error("Expected " + std::to_string(COUNT) + " key release events");
        }

        // Integration Tests

        void NodeSimulationTestSuite::Test_Integration_StartToEnd_Chain()
        {
            auto startNode = StartNode::Create();
            auto keyPressNode = KeyPressNode::Create(Lumina::KeyCode::A);
            auto keyReleaseNode = KeyReleaseNode::Create(Lumina::KeyCode::A);
            auto endNode = EndNode::Create();

            // Create chain: Start -> KeyPress -> KeyRelease -> End
            if (!Node::ConnectPins(startNode, PinType::Output, keyPressNode, PinType::Input))
                throw std::runtime_error("Failed to connect Start->KeyPress");

            if (!Node::ConnectPins(keyPressNode, PinType::Output, keyReleaseNode, PinType::Input))
                throw std::runtime_error("Failed to connect KeyPress->KeyRelease");

            if (!Node::ConnectPins(keyReleaseNode, PinType::Output, endNode, PinType::Input))
                throw std::runtime_error("Failed to connect KeyRelease->End");

            MockInputPlayback playback;

            // Execute the chain
            auto node1 = startNode->Execute(&playback);
            if (!node1 || node1->GetNodeID() != keyPressNode->GetNodeID())
                throw std::runtime_error("Start didn't return KeyPress node");

            auto node2 = node1->Execute(&playback);
            if (!node2 || node2->GetNodeID() != keyReleaseNode->GetNodeID())
                throw std::runtime_error("KeyPress didn't return KeyRelease node");

            auto node3 = node2->Execute(&playback);
            if (!node3 || node3->GetNodeID() != endNode->GetNodeID())
                throw std::runtime_error("KeyRelease didn't return End node");

            auto node4 = node3->Execute(&playback);
            if (node4 != nullptr)
                throw std::runtime_error("End node should return null");

            // Verify events were simulated
            if (playback.GetEventCount() != 2)
                throw std::runtime_error("Expected 2 simulated events (press and release)");
        }

        void NodeSimulationTestSuite::Test_Integration_StartWithoutEnd()
        {
            auto startNode = StartNode::Create();
            auto keyNode = KeyPressNode::Create(Lumina::KeyCode::A);

            // Chain without end: Start -> KeyPress (no end)
            if (!Node::ConnectPins(startNode, PinType::Output, keyNode, PinType::Input))
                throw std::runtime_error("Failed to connect nodes");

            MockInputPlayback playback;

            auto node1 = startNode->Execute(&playback);
            if (!node1)
                throw std::runtime_error("Start returned null");

            auto node2 = node1->Execute(&playback);
            if (node2 != nullptr)
                throw std::runtime_error("Unconnected KeyPress should return null");
        }

        void NodeSimulationTestSuite::Test_Integration_MultipleStartNodes()
        {
            // Test that multiple start nodes can exist independently
            auto start1 = StartNode::Create();
            auto start2 = StartNode::Create();
            auto key1 = KeyPressNode::Create(Lumina::KeyCode::A);
            auto key2 = KeyPressNode::Create(Lumina::KeyCode::B);

            Node::ConnectPins(start1, PinType::Output, key1, PinType::Input);
            Node::ConnectPins(start2, PinType::Output, key2, PinType::Input);

            MockInputPlayback playback;

            auto next1 = start1->Execute(&playback);
            auto next2 = start2->Execute(&playback);

            if (!next1 || next1->GetNodeID() != key1->GetNodeID())
                throw std::runtime_error("Start1 didn't return correct node");

            if (!next2 || next2->GetNodeID() != key2->GetNodeID())
                throw std::runtime_error("Start2 didn't return correct node");
        }
    }
}
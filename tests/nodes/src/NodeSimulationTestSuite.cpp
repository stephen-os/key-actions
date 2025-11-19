#include "NodeSimulationTestSuite.h"

namespace KeyActions::Testing
{
    std::vector<TestResult> NodeSimulationTestSuite::RunAllTests()
    {
        m_LastSummary = TestSummary();
        m_LastSummary.Results.clear();

        LUMINA_LOG_INFO("========================================");
        LUMINA_LOG_INFO("Running Node Simulation Test Suite");
        LUMINA_LOG_INFO("========================================");

        Lumina::Timer totalTimer;

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

        // Performance Tests
        m_LastSummary.Results.push_back(RunTest("Performance - Create Many KeyPressNodes", [this]() { Test_Performance_KeyPressNode_CreateMany(); }));
        m_LastSummary.Results.push_back(RunTest("Performance - Execute Many KeyPressNodes", [this]() { Test_Performance_KeyPressNode_ExecuteMany(); }));

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

    // ============================================================
    // KeyPressNode Tests
    // ============================================================

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

    void NodeSimulationTestSuite::Test_KeyPressNode_Execute_WithNullPlayback()
    {
        auto node = KeyPressNode::Create(Lumina::KeyCode::A);

        bool exceptionThrown = false;
        try
        {
            node->Execute(nullptr);
        }
        catch (...)
        {
            exceptionThrown = true;
        }

        if (!exceptionThrown)
            throw std::runtime_error("Execute with null playback should throw exception");
    }

    // ============================================================
    // Performance Tests
    // ============================================================

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

} // namespace KeyActions::Testing
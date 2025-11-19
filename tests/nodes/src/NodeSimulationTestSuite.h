#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "KeyActions/Core/Nodes/Node.h"
#include "KeyActions/Core/Nodes/KeyPressNode.h"
#include "Lumina/Utils/Timer.h"
#include "Lumina/Input/GlobalInputPlayback.h"

namespace KeyActions::Testing
{
    struct TestResult
    {
        std::string TestName;
        bool Passed;
        std::string Message;
        float ElapsedMs;
    };

    class MockInputPlayback : public Lumina::GlobalInputPlayback
    {
    public:
        struct SimulatedEvent
        {
            enum class Type
            {
                KeyPress,
                KeyRelease,
                MouseButtonPress,
                MouseButtonRelease,
                MouseMove,
                MouseScroll
            };

            Type EventType;
            Lumina::KeyCode Key = Lumina::KeyCode::Unknown;
            Lumina::MouseCode MouseButton = Lumina::MouseCode::Button0;
            int X = 0;
            int Y = 0;
            int DX = 0;
            int DY = 0;
        };

        void SimulateKeyPress(Lumina::KeyCode key) override
        {
            SimulatedEvent event;
            event.EventType = SimulatedEvent::Type::KeyPress;
            event.Key = key;
            m_Events.push_back(event);
        }

        void SimulateKeyRelease(Lumina::KeyCode key) override
        {
            SimulatedEvent event;
            event.EventType = SimulatedEvent::Type::KeyRelease;
            event.Key = key;
            m_Events.push_back(event);
        }

        void SimulateMouseButtonPress(Lumina::MouseCode button, int x, int y) override
        {
            SimulatedEvent event;
            event.EventType = SimulatedEvent::Type::MouseButtonPress;
            event.MouseButton = button;
            event.X = x;
            event.Y = y;
            m_Events.push_back(event);
        }

        void SimulateMouseButtonRelease(Lumina::MouseCode button, int x, int y) override
        {
            SimulatedEvent event;
            event.EventType = SimulatedEvent::Type::MouseButtonRelease;
            event.MouseButton = button;
            event.X = x;
            event.Y = y;
            m_Events.push_back(event);
        }

        void SimulateMouseMove(int x, int y) override
        {
            SimulatedEvent event;
            event.EventType = SimulatedEvent::Type::MouseMove;
            event.X = x;
            event.Y = y;
            m_Events.push_back(event);
        }

        void SimulateMouseScroll(int dx, int dy) override
        {
            SimulatedEvent event;
            event.EventType = SimulatedEvent::Type::MouseScroll;
            event.DX = dx;
            event.DY = dy;
            m_Events.push_back(event);
        }

        const std::vector<SimulatedEvent>& GetEvents() const { return m_Events; }
        void ClearEvents() { m_Events.clear(); }
        size_t GetEventCount() const { return m_Events.size(); }

        bool HasKeyPress(Lumina::KeyCode key) const
        {
            for (const auto& event : m_Events)
            {
                if (event.EventType == SimulatedEvent::Type::KeyPress && event.Key == key)
                    return true;
            }
            return false;
        }

        bool HasKeyRelease(Lumina::KeyCode key) const
        {
            for (const auto& event : m_Events)
            {
                if (event.EventType == SimulatedEvent::Type::KeyRelease && event.Key == key)
                    return true;
            }
            return false;
        }

    private:
        std::vector<SimulatedEvent> m_Events;
    };

    class NodeSimulationTestSuite
    {
    public:
        NodeSimulationTestSuite() = default;

        std::vector<TestResult> RunAllTests();

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

        TestResult RunTest(const std::string& name, std::function<void()> testFunc);

        // KeyPressNode Tests
        void Test_KeyPressNode_Creation();
        void Test_KeyPressNode_GetKey();
        void Test_KeyPressNode_GetName();
        void Test_KeyPressNode_GetType();
        void Test_KeyPressNode_HasPins();
        void Test_KeyPressNode_CanConnect();
        void Test_KeyPressNode_Execute_SimulatesKeyPress();
        void Test_KeyPressNode_Execute_ReturnsConnectedNode();
        void Test_KeyPressNode_Execute_ReturnsNullWhenNotConnected();
        void Test_KeyPressNode_Execute_WithNullPlayback();

        // Performance Tests
        void Test_Performance_KeyPressNode_CreateMany();
        void Test_Performance_KeyPressNode_ExecuteMany();
    };

} // namespace KeyActions::Testing
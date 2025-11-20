#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "KeyActions/Core/Nodes/Node.h"
#include "KeyActions/Core/Nodes/KeyPressNode.h"
#include "KeyActions/Core/Nodes/KeyReleaseNode.h"

namespace KeyActions
{
    namespace Tests
    {
        struct TestResult
        {
            std::string TestName;
            bool Passed;
            std::string Message;
            float ElapsedMs;
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
            void Test_Performance_KeyPressNode_CreateMany();
            void Test_Performance_KeyPressNode_ExecuteMany();
    
            void Test_KeyReleaseNode_Creation();
            void Test_KeyReleaseNode_GetKey();
            void Test_KeyReleaseNode_GetName();
            void Test_KeyReleaseNode_GetType();
            void Test_KeyReleaseNode_HasPins();
            void Test_KeyReleaseNode_CanConnect();
            void Test_KeyReleaseNode_Execute_SimulatesKeyRelease();
            void Test_KeyReleaseNode_Execute_ReturnsConnectedNode();
            void Test_KeyReleaseNode_Execute_ReturnsNullWhenNotConnected();
            void Test_KeyReleaseNode_Execute_WithNullPlayback();
            void Test_Performance_KeyReleaseNode_CreateMany();
            void Test_Performance_KeyReleaseNode_ExecuteMany();
        };
    }
}
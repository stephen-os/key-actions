#pragma once

#include "Lumina/Core/Layer.h"
#include "NodeSimulationTestSuite.h"

namespace Lumina
{
    class NodeSimulationTestLayer : public Layer
    {
    public:
        NodeSimulationTestLayer()
            : Layer("NodeSimulationTestLayer")
        {
        }

        virtual void OnAttach() override
        {
            LUMINA_LOG_INFO("========================================");
            LUMINA_LOG_INFO("NodeSimulationTestLayer Attached");
            LUMINA_LOG_INFO("========================================");

            if (m_RunTestsOnStartup)
            {
                LUMINA_LOG_INFO("Running tests on startup...");
                m_TestSuite.RunAllTests();
            }
        }

        virtual void OnDetach() override
        {
            LUMINA_LOG_INFO("NodeSimulationTestLayer Detached");
        }

        virtual void OnUpdate(float timestep) override
        {
            // Tests don't need to update every frame
        }

        virtual void OnUIRender() override
        {
            RenderTestControlPanel();
            RenderTestResults();
        }

    private:
        void RenderTestControlPanel()
        {
            ImGui::Begin("Node Simulation Test Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

            // Title
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
            ImGui::TextWrapped("Node Simulation Test Suite");
            ImGui::PopStyleColor();

            ImGui::Separator();

            // Run Tests Button
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.4f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.3f, 0.0f, 1.0f));

            if (ImGui::Button("Run All Node Tests", ImVec2(200, 40)))
            {
                LUMINA_LOG_INFO("========================================");
                LUMINA_LOG_INFO("User triggered node test suite execution");
                LUMINA_LOG_INFO("========================================");
                m_TestSuite.RunAllTests();
            }

            ImGui::PopStyleColor(3);

            // Summary Statistics
            auto summary = m_TestSuite.GetLastSummary();
            if (summary.TotalTests > 0)
            {
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                // Pass/Fail indicator
                bool allPassed = summary.FailedTests == 0;
                ImVec4 statusColor = allPassed
                    ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f)  // Green
                    : ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red

                ImGui::PushStyleColor(ImGuiCol_Text, statusColor);
                ImGui::Text("Status: %s", allPassed ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
                ImGui::PopStyleColor();

                ImGui::Spacing();

                // Statistics
                ImGui::Text("Total Tests:    %d", summary.TotalTests);

                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),
                    "Passed:         %d", summary.PassedTests);

                if (summary.FailedTests > 0)
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                        "Failed:         %d", summary.FailedTests);
                }

                ImGui::Text("Total Time:     %.3f ms", summary.TotalTimeMs);

                float avgTime = summary.TotalTests > 0
                    ? summary.TotalTimeMs / summary.TotalTests
                    : 0.0f;
                ImGui::Text("Average Time:   %.3f ms", avgTime);

                ImGui::Spacing();

                // Pass rate progress bar
                float passRate = summary.TotalTests > 0
                    ? (float)summary.PassedTests / summary.TotalTests
                    : 0.0f;

                ImGui::Text("Pass Rate:");
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
                    passRate >= 1.0f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
                ImGui::ProgressBar(passRate, ImVec2(-1.0f, 0.0f));
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::Text("%.1f%%", passRate * 100.0f);
            }
            else
            {
                ImGui::Spacing();
                ImGui::TextWrapped("No tests have been run yet.");
                ImGui::Spacing();
                ImGui::TextWrapped("Click 'Run All Node Tests' to execute the test suite.");
            }

            ImGui::Spacing();
            ImGui::Separator();

            // Options
            ImGui::Text("Options:");
            ImGui::Checkbox("Run tests on startup", &m_RunTestsOnStartup);
            ImGui::Checkbox("Show detailed results", &m_ShowDetailedResults);

            if (ImGui::Checkbox("Show only failures", &m_ShowOnlyFailures))
            {
                if (m_ShowOnlyFailures)
                    m_ShowDetailedResults = true;
            }

            ImGui::Spacing();
            ImGui::Separator();

            // Test Info
            ImGui::Text("Test Coverage:");
            ImGui::BulletText("KeyPressNode - Creation & Properties");
            ImGui::BulletText("KeyPressNode - Pin Configuration");
            ImGui::BulletText("KeyPressNode - Connection Rules");
            ImGui::BulletText("KeyPressNode - Execution & Simulation");
            ImGui::BulletText("KeyPressNode - Performance Benchmarks");

            ImGui::End();
        }

        void RenderTestResults()
        {
            auto summary = m_TestSuite.GetLastSummary();

            if (summary.TotalTests == 0 || !m_ShowDetailedResults)
                return;

            ImGui::Begin("Node Simulation Test Results", &m_ShowDetailedResults,
                ImGuiWindowFlags_HorizontalScrollbar);

            // Filter controls
            ImGui::Text("Filter:");
            ImGui::SameLine();

            if (ImGui::Button("All"))
            {
                m_FilterCategory = TestCategory::All;
            }
            ImGui::SameLine();
            if (ImGui::Button("Creation"))
            {
                m_FilterCategory = TestCategory::Creation;
            }
            ImGui::SameLine();
            if (ImGui::Button("Properties"))
            {
                m_FilterCategory = TestCategory::Properties;
            }
            ImGui::SameLine();
            if (ImGui::Button("Execution"))
            {
                m_FilterCategory = TestCategory::Execution;
            }
            ImGui::SameLine();
            if (ImGui::Button("Performance"))
            {
                m_FilterCategory = TestCategory::Performance;
            }

            ImGui::Separator();

            // Results table
            if (ImGui::BeginTable("NodeTestResultsTable", 4,
                ImGuiTableFlags_Borders |
                ImGuiTableFlags_RowBg |
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_Sortable |
                ImGuiTableFlags_ScrollY,
                ImVec2(0.0f, 500.0f)))
            {
                // Setup columns
                ImGui::TableSetupColumn("Test Name",
                    ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultSort);
                ImGui::TableSetupColumn("Status",
                    ImGuiTableColumnFlags_WidthFixed, 80.0f);
                ImGui::TableSetupColumn("Time (ms)",
                    ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Message",
                    ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableHeadersRow();

                // Display results
                for (const auto& result : summary.Results)
                {
                    // Apply filters
                    if (m_ShowOnlyFailures && result.Passed)
                        continue;

                    if (m_FilterCategory != TestCategory::All &&
                        !MatchesCategory(result.TestName, m_FilterCategory))
                        continue;

                    ImGui::TableNextRow();

                    // Test Name
                    ImGui::TableNextColumn();
                    ImGui::TextWrapped("%s", result.TestName.c_str());

                    // Status
                    ImGui::TableNextColumn();
                    if (result.Passed)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                        ImGui::Text("PASS");
                        ImGui::PopStyleColor();
                    }
                    else
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                        ImGui::Text("FAIL");
                        ImGui::PopStyleColor();
                    }

                    // Time
                    ImGui::TableNextColumn();

                    // Color code based on performance
                    ImVec4 timeColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    if (result.ElapsedMs < 1.0f)
                        timeColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                    else if (result.ElapsedMs > 10.0f)
                        timeColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
                    else if (result.ElapsedMs > 100.0f)
                        timeColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

                    ImGui::TextColored(timeColor, "%.3f", result.ElapsedMs);

                    // Message
                    ImGui::TableNextColumn();
                    if (!result.Passed)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.7f, 0.7f, 1.0f));
                        ImGui::TextWrapped("%s", result.Message.c_str());
                        ImGui::PopStyleColor();
                    }
                    else
                    {
                        ImGui::TextWrapped("%s", result.Message.c_str());
                    }
                }

                ImGui::EndTable();
            }

            ImGui::Spacing();
            ImGui::Separator();

            // Export buttons
            if (ImGui::Button("Copy Results to Clipboard"))
            {
                std::string results = GenerateResultsReport(summary);
                ImGui::SetClipboardText(results.c_str());
                LUMINA_LOG_INFO("Test results copied to clipboard");
            }

            ImGui::SameLine();

            if (ImGui::Button("Export to Console"))
            {
                LUMINA_LOG_INFO("========================================");
                LUMINA_LOG_INFO("NODE TEST RESULTS EXPORT");
                LUMINA_LOG_INFO("========================================");
                for (const auto& result : summary.Results)
                {
                    if (result.Passed)
                        LUMINA_LOG_INFO("[PASS] {} - {:.3f}ms", result.TestName, result.ElapsedMs);
                    else
                        LUMINA_LOG_ERROR("[FAIL] {} - {} - {:.3f}ms",
                            result.TestName, result.Message, result.ElapsedMs);
                }
                LUMINA_LOG_INFO("========================================");
            }

            ImGui::End();
        }

        enum class TestCategory
        {
            All,
            Creation,
            Properties,
            Execution,
            Performance
        };

        bool MatchesCategory(const std::string& testName, TestCategory category)
        {
            if (category == TestCategory::All)
                return true;

            switch (category)
            {
            case TestCategory::Creation:
                return testName.find("Creation") != std::string::npos;

            case TestCategory::Properties:
                return testName.find("Get Key") != std::string::npos ||
                    testName.find("Get Name") != std::string::npos ||
                    testName.find("Get Type") != std::string::npos ||
                    testName.find("Has Pins") != std::string::npos ||
                    testName.find("Can Connect") != std::string::npos;

            case TestCategory::Execution:
                return testName.find("Execute") != std::string::npos ||
                    testName.find("Simulates") != std::string::npos ||
                    testName.find("Returns") != std::string::npos;

            case TestCategory::Performance:
                return testName.find("Performance") != std::string::npos;

            default:
                return true;
            }
        }

        std::string GenerateResultsReport(const KeyActions::Tests::NodeSimulationTestSuite::TestSummary& summary)
        {
            std::stringstream ss;

            ss << "========================================\n";
            ss << "NODE SIMULATION TEST SUITE RESULTS\n";
            ss << "========================================\n\n";

            ss << "Summary:\n";
            ss << "  Total Tests:  " << summary.TotalTests << "\n";
            ss << "  Passed:       " << summary.PassedTests << "\n";
            ss << "  Failed:       " << summary.FailedTests << "\n";
            ss << "  Total Time:   " << summary.TotalTimeMs << " ms\n";

            float passRate = summary.TotalTests > 0
                ? (float)summary.PassedTests / summary.TotalTests * 100.0f
                : 0.0f;
            ss << "  Pass Rate:    " << passRate << "%\n\n";

            ss << "Detailed Results:\n";
            ss << "----------------------------------------\n";

            for (const auto& result : summary.Results)
            {
                ss << (result.Passed ? "[PASS] " : "[FAIL] ");
                ss << result.TestName;
                ss << " (" << result.ElapsedMs << " ms)";

                if (!result.Passed)
                {
                    ss << "\n  Error: " << result.Message;
                }

                ss << "\n";
            }

            ss << "========================================\n";

            return ss.str();
        }

    private:
        KeyActions::Tests::NodeSimulationTestSuite m_TestSuite;

        // UI State
        bool m_RunTestsOnStartup = false;
        bool m_ShowDetailedResults = true;
        bool m_ShowOnlyFailures = false;
        TestCategory m_FilterCategory = TestCategory::All;
    };
}
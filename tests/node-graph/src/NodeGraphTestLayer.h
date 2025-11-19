#pragma once

#include "Lumina/Core/Layer.h"
#include "NodeGraphTestSuite.h"

namespace Lumina
{
    class NodeGraphTestLayer : public Layer
    {
    public:
        NodeGraphTestLayer()
            : Layer("NodeGraphTestLayer")
        {
        }

        virtual void OnAttach() override
        {
            LUMINA_LOG_INFO("========================================");
            LUMINA_LOG_INFO("NodeGraphTestLayer Attached");
            LUMINA_LOG_INFO("========================================");

            // Optionally run tests automatically on startup
            if (m_RunTestsOnStartup)
            {
                LUMINA_LOG_INFO("Running tests on startup...");
                m_TestSuite.RunAllTests();
            }
        }

        virtual void OnDetach() override
        {
            LUMINA_LOG_INFO("NodeGraphTestLayer Detached");
        }

        virtual void OnUpdate(float timestep) override
        {
            // Tests don't need to update every frame
            // Can add continuous stress testing here if needed
        }

        virtual void OnUIRender() override
        {
            RenderTestControlPanel();
            RenderTestResults();
        }

    private:
        void RenderTestControlPanel()
        {
            ImGui::Begin("NodeGraph Test Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

            // Title
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
            ImGui::TextWrapped("NodeGraph Test Suite");
            ImGui::PopStyleColor();

            ImGui::Separator();

            // Run Tests Button
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));

            if (ImGui::Button("Run All Tests", ImVec2(200, 40)))
            {
                LUMINA_LOG_INFO("========================================");
                LUMINA_LOG_INFO("User triggered test suite execution");
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
                ImGui::TextWrapped("Click 'Run All Tests' to execute the test suite.");
            }

            ImGui::Spacing();
            ImGui::Separator();

            // Options
            ImGui::Text("Options:");
            ImGui::Checkbox("Run tests on startup", &m_RunTestsOnStartup);
            ImGui::Checkbox("Show detailed results", &m_ShowDetailedResults);

            if (ImGui::Checkbox("Show only failures", &m_ShowOnlyFailures))
            {
                // If showing only failures, must show detailed results
                if (m_ShowOnlyFailures)
                    m_ShowDetailedResults = true;
            }

            ImGui::End();
        }

        void RenderTestResults()
        {
            auto summary = m_TestSuite.GetLastSummary();

            // Only show if tests have been run and user wants detailed results
            if (summary.TotalTests == 0 || !m_ShowDetailedResults)
                return;

            ImGui::Begin("NodeGraph Test Results", &m_ShowDetailedResults,
                ImGuiWindowFlags_HorizontalScrollbar);

            // Filter controls
            ImGui::Text("Filter:");
            ImGui::SameLine();

            // Category filter buttons
            if (ImGui::Button("All"))
            {
                m_FilterCategory = TestCategory::All;
            }
            ImGui::SameLine();
            if (ImGui::Button("Node Mgmt"))
            {
                m_FilterCategory = TestCategory::NodeManagement;
            }
            ImGui::SameLine();
            if (ImGui::Button("Connections"))
            {
                m_FilterCategory = TestCategory::Connections;
            }
            ImGui::SameLine();
            if (ImGui::Button("Queries"))
            {
                m_FilterCategory = TestCategory::Queries;
            }
            ImGui::SameLine();
            if (ImGui::Button("Analysis"))
            {
                m_FilterCategory = TestCategory::Analysis;
            }
            ImGui::SameLine();
            if (ImGui::Button("Performance"))
            {
                m_FilterCategory = TestCategory::Performance;
            }

            ImGui::Separator();

            // Results table
            if (ImGui::BeginTable("TestResultsTable", 4,
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
                ImGui::TableSetupScrollFreeze(0, 1); // Freeze header row
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
                    ImVec4 timeColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White (normal)
                    if (result.ElapsedMs < 1.0f)
                        timeColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green (fast)
                    else if (result.ElapsedMs > 10.0f)
                        timeColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f); // Orange (slow)
                    else if (result.ElapsedMs > 100.0f)
                        timeColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red (very slow)

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
                LUMINA_LOG_INFO("TEST RESULTS EXPORT");
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
            NodeManagement,
            Connections,
            Queries,
            Analysis,
            Validation,
            Performance
        };

        bool MatchesCategory(const std::string& testName, TestCategory category)
        {
            if (category == TestCategory::All)
                return true;

            switch (category)
            {
            case TestCategory::NodeManagement:
                return testName.find("Add") != std::string::npos ||
                    testName.find("Remove") != std::string::npos ||
                    testName.find("Get") != std::string::npos ||
                    testName.find("Has") != std::string::npos ||
                    testName.find("Count") != std::string::npos ||
                    testName.find("Empty") != std::string::npos ||
                    testName.find("Clear") != std::string::npos;

            case TestCategory::Connections:
                return testName.find("Connect") != std::string::npos ||
                    testName.find("Disconnect") != std::string::npos;

            case TestCategory::Queries:
                return testName.find("Get Connected") != std::string::npos ||
                    testName.find("Incoming") != std::string::npos ||
                    testName.find("Outgoing") != std::string::npos ||
                    testName.find("Are Nodes") != std::string::npos;

            case TestCategory::Analysis:
                return testName.find("Cycle") != std::string::npos ||
                    testName.find("Topological") != std::string::npos ||
                    testName.find("Path") != std::string::npos ||
                    testName.find("Reachable") != std::string::npos;

            case TestCategory::Validation:
                return testName.find("Validate") != std::string::npos ||
                    testName.find("Would Create") != std::string::npos;

            case TestCategory::Performance:
                return testName.find("Performance") != std::string::npos;

            default:
                return true;
            }
        }

        std::string GenerateResultsReport(const KeyActions::Testing::NodeGraphTestSuite::TestSummary& summary)
        {
            std::stringstream ss;

            ss << "========================================\n";
            ss << "NODEGRAPH TEST SUITE RESULTS\n";
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
        KeyActions::Testing::NodeGraphTestSuite m_TestSuite;

        // UI State
        bool m_RunTestsOnStartup = false;
        bool m_ShowDetailedResults = true;
        bool m_ShowOnlyFailures = false;
        TestCategory m_FilterCategory = TestCategory::All;
    };
}
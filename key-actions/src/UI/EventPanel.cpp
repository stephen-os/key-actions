// UI/EventPanel.cpp
#include "EventPanel.h"
#include "Lumina/Core/Input.h"
#include <sstream>
#include <iomanip>

namespace Lumina
{
    EventPanel::EventPanel(int maxEvents)
        : m_MaxEvents(maxEvents)
    {
    }

    void EventPanel::AddEvent(const RecordedEvent& event)
    {
        m_Events.push_back(event);

        // Limit event count
        if (m_Events.size() > m_MaxEvents)
        {
            m_Events.erase(m_Events.begin());
        }
    }

    void EventPanel::Clear()
    {
        m_Events.clear();
    }

    ImVec4 EventPanel::GetEventColor(ActionType action) const
    {
        switch (action)
        {
        case ActionType::KeyPressed:       return ImVec4(0.4f, 0.8f, 0.4f, 1.0f); // Green
        case ActionType::KeyReleased:      return ImVec4(0.8f, 0.4f, 0.4f, 1.0f); // Red
        case ActionType::MousePressed:     return ImVec4(0.4f, 0.6f, 0.9f, 1.0f); // Blue
        case ActionType::MouseReleased:    return ImVec4(0.7f, 0.4f, 0.8f, 1.0f); // Purple
        case ActionType::MouseMoved:       return ImVec4(0.6f, 0.6f, 0.6f, 1.0f); // Gray
        case ActionType::MouseScrolled:    return ImVec4(0.9f, 0.7f, 0.4f, 1.0f); // Orange
        default:                           return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
        }
    }

    const char* EventPanel::GetEventIcon(ActionType action) const
    {
        switch (action)
        {
        case ActionType::KeyPressed:       return "[KEY+]";
        case ActionType::KeyReleased:      return "[KEY-]";
        case ActionType::MousePressed:     return "[MOUSE+]";
        case ActionType::MouseReleased:    return "[MOUSE-]";
        case ActionType::MouseMoved:       return "[MOVE]";
        case ActionType::MouseScrolled:    return "[SCROLL]";
        default:                           return "[???]";
        }
    }

    void EventPanel::RenderEvent(const RecordedEvent& event, int index)
    {
        ImGui::PushID(index);

        // Format time as MM:SS.mmm
        int minutes = static_cast<int>(event.Time) / 60;
        int seconds = static_cast<int>(event.Time) % 60;
        int milliseconds = static_cast<int>((event.Time - static_cast<int>(event.Time)) * 1000);

        std::stringstream timeStr;
        timeStr << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setw(2) << seconds << "."
            << std::setw(3) << milliseconds;

        // Create a button-like appearance for each event
        ImVec4 color = GetEventColor(event.Action);
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.x * 1.2f, color.y * 1.2f, color.z * 1.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

        // Time column
        ImGui::Button(timeStr.str().c_str(), ImVec2(80, 0));
        ImGui::SameLine();

        // Event type icon
        ImGui::Button(GetEventIcon(event.Action), ImVec2(80, 0));
        ImGui::SameLine();

        // Event details
        std::string details;
        switch (event.Action)
        {
        case ActionType::KeyPressed:
        case ActionType::KeyReleased:
            details = Input::KeyCodeToString(event.Key);
            break;
        case ActionType::MousePressed:
        case ActionType::MouseReleased:
        {
            std::stringstream ss;
            ss << "Button " << static_cast<int>(event.Button)
                << " at (" << static_cast<int>(event.MouseX)
                << ", " << static_cast<int>(event.MouseY) << ")";
            details = ss.str();
            break;
        }
        case ActionType::MouseMoved:
        {
            std::stringstream ss;
            ss << "(" << static_cast<int>(event.MouseX)
                << ", " << static_cast<int>(event.MouseY) << ")";
            details = ss.str();
            break;
        }
        case ActionType::MouseScrolled:
        {
            std::stringstream ss;
            ss << "dx=" << event.ScrollDX << ", dy=" << event.ScrollDY;
            details = ss.str();
            break;
        }
        }

        ImGui::Button(details.c_str(), ImVec2(-1, 0));

        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }

    void EventPanel::Render(const ImVec2& size)
    {
        ImGui::BeginChild("EventPanelEvents", ImVec2(size.x, size.y - 30), false);

        for (size_t i = 0; i < m_Events.size(); i++)
        {
            RenderEvent(m_Events[i], static_cast<int>(i));
        }

        // Auto-scroll to bottom
        if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();

        // Clear button
        if (ImGui::Button("Clear Events", ImVec2(-1, 0)))
        {
            Clear();
        }
    }
}
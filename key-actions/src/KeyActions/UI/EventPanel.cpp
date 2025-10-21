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

    ImVec4 EventPanel::GetEventColor(RecordedAction action) const
    {
        switch (action)
        {
        case RecordedAction::KeyPressed:       return ImVec4(0.4f, 0.8f, 0.4f, 1.0f); // Green
        case RecordedAction::KeyReleased:      return ImVec4(0.8f, 0.4f, 0.4f, 1.0f); // Red
        case RecordedAction::MousePressed:     return ImVec4(0.4f, 0.6f, 0.9f, 1.0f); // Blue
        case RecordedAction::MouseReleased:    return ImVec4(0.7f, 0.4f, 0.8f, 1.0f); // Purple
        case RecordedAction::MouseMoved:       return ImVec4(0.6f, 0.6f, 0.6f, 1.0f); // Gray
        case RecordedAction::MouseScrolled:    return ImVec4(0.9f, 0.7f, 0.4f, 1.0f); // Orange
        default:                           return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
        }
    }

    const char* EventPanel::GetEventIcon(RecordedAction action) const
    {
        switch (action)
        {
        case RecordedAction::KeyPressed:       return "[KEY+]";
        case RecordedAction::KeyReleased:      return "[KEY-]";
        case RecordedAction::MousePressed:     return "[MOUSE+]";
        case RecordedAction::MouseReleased:    return "[MOUSE-]";
        case RecordedAction::MouseMoved:       return "[MOVE]";
        case RecordedAction::MouseScrolled:    return "[SCROLL]";
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
        case RecordedAction::KeyPressed:
        case RecordedAction::KeyReleased:
            details = Input::KeyCodeToString(event.Key);
            break;
        case RecordedAction::MousePressed:
        case RecordedAction::MouseReleased:
        {
            std::stringstream ss;
            ss << "Button " << static_cast<int>(event.Button)
                << " at (" << static_cast<int>(event.MouseX)
                << ", " << static_cast<int>(event.MouseY) << ")";
            details = ss.str();
            break;
        }
        case RecordedAction::MouseMoved:
        {
            std::stringstream ss;
            ss << "(" << static_cast<int>(event.MouseX)
                << ", " << static_cast<int>(event.MouseY) << ")";
            details = ss.str();
            break;
        }
        case RecordedAction::MouseScrolled:
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
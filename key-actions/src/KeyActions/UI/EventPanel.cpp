#include "EventPanel.h"

#include "Lumina/Core/Input.h"
#include "Styles/StyledWidgets.h"
#include "Styles/Theme.h"

#include <sstream>
#include <iomanip>

namespace KeyActions
{
    EventPanel::EventPanel(int maxEvents) : m_MaxEvents(maxEvents) {}

    void EventPanel::AddEvent(const RecordedEvent& event)
    {
        m_Events.push_back(event);

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
        using namespace UI;

        switch (action)
        {
        case RecordedAction::KeyPressed:       return Colors::KeyPressed;
        case RecordedAction::KeyReleased:      return Colors::KeyReleased;
        case RecordedAction::MousePressed:     return Colors::MousePressed;
        case RecordedAction::MouseReleased:    return Colors::MouseReleased;
        case RecordedAction::MouseMoved:       return Colors::MouseMoved;
        case RecordedAction::MouseScrolled:    return Colors::MouseScrolled;
        default:                               return Colors::TextNormal;
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
        default:                               return "[???]";
        }
    }

    void EventPanel::RenderEvent(const RecordedEvent& event, int index)
    {
        using namespace UI;

        ImGui::PushID(index);

        int minutes = static_cast<int>(event.Time) / 60;
        int seconds = static_cast<int>(event.Time) % 60;
        int milliseconds = static_cast<int>((event.Time - static_cast<int>(event.Time)) * 1000);

        std::stringstream timeStr;
        timeStr << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setw(2) << seconds << "."
            << std::setw(3) << milliseconds;

        ImVec4 color = GetEventColor(event.Action);
        ImVec4 hoverColor = ImVec4(
            std::min(color.x * 1.2f, 1.0f),
            std::min(color.y * 1.2f, 1.0f),
            std::min(color.z * 1.2f, 1.0f),
            1.0f
        );

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Small);

        ImGui::Button(timeStr.str().c_str(), ImVec2(90, 26));
        ImGui::SameLine();

        ImGui::Button(GetEventIcon(event.Action), ImVec2(90, 26));
        ImGui::SameLine();

        std::string details;
        switch (event.Action)
        {
        case RecordedAction::KeyPressed:
        case RecordedAction::KeyReleased:
            details = Lumina::Input::KeyCodeToString(event.Key);
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

        ImGui::Button(details.c_str(), ImVec2(-1, 26));

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }

    void EventPanel::Render(const ImVec2& size)
    {
        using namespace UI;

        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, Rounding::Small);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors::BackgroundDark);
        ImGui::BeginChild("EventPanelEvents", ImVec2(size.x, size.y - 45), true);

        for (size_t i = 0; i < m_Events.size(); i++)
        {
            RenderEvent(m_Events[i], static_cast<int>(i));
        }

        if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        if (ButtonDanger("Clear Events", Sizes::ButtonFull))
        {
            Clear();
        }
    }
}
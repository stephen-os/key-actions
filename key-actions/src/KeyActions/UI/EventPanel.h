#pragma once

#include <vector>
#include <imgui.h>

#include "KeyActions/Core/Recording.h"

namespace KeyActions
{
    class EventPanel
    {
    public:
        EventPanel(int maxEvents = 1000);

        void AddEvent(const RecordedEvent& event);
        void Clear();
        void Render(const ImVec2& size = ImVec2(0, 0));

        size_t GetEventCount() const { return m_Events.size(); }

    private:
        void RenderEvent(const RecordedEvent& event, int index);
        ImVec4 GetEventColor(RecordedAction action) const;
        const char* GetEventIcon(RecordedAction action) const;

    private:
        std::vector<RecordedEvent> m_Events;
        int m_MaxEvents;
        bool m_AutoScroll = true;
    };
}
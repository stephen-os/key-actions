#pragma once

#include <string>
#include <vector>

#include "Lumina/Core/KeyCodes.h"

namespace Lumina
{
    enum class ActionType
    {
        KeyPressed,
        KeyReleased,
        MousePressed,
        MouseReleased,
        MouseMoved,
        MouseScrolled
    };

    struct RecordedEvent
    {
        ActionType Action;
        float Time;  // Timestamp relative to recording start

        // Key events
        KeyCode Key = KeyCode::Unknown;

        // Mouse events
        MouseCode Button = MouseCode::Button0;
        float MouseX = 0.0f;
        float MouseY = 0.0f;
        int ScrollDX = 0;
        int ScrollDY = 0;

        std::string ToString() const;
    };

    struct Recording
    {
        std::string Name;
        std::vector<RecordedEvent> Events;
        float TotalDuration = 0.0f;
        bool RecordsMouse = false;

        Recording() = default;
        Recording(const std::string& name, bool recordMouse = false)
            : Name(name), RecordsMouse(recordMouse) {
        }
    };
}
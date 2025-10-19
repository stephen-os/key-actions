#pragma once

#include <string>
#include <vector>

#include "Lumina/Core/KeyCodes.h"

namespace Lumina
{
    enum class RecordedAction
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
        RecordedAction Action;
        float Time;

        KeyCode Key = KeyCode::Unknown;
        bool ShiftPressed = false;
        bool CtrlPressed = false;
        bool AltPressed = false;
        bool SuperPressed = false;
        bool CapsLockActive = false;

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
        Recording(const std::string& name, bool recordMouse = false) : Name(name), RecordsMouse(recordMouse) {}
    };
}
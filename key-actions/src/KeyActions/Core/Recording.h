#pragma once

#include <string>
#include <vector>

#include "Lumina/Core/KeyCodes.h"

namespace KeyActions
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
        using KeyCode = Lumina::KeyCode;
		using MouseCode = Lumina::MouseCode;

        RecordedAction Action;
        float Time;

        KeyCode Key = KeyCode::Unknown;
        bool ShiftPressed = false;
        bool CtrlPressed = false;
        bool AltPressed = false;
        bool SuperPressed = false;
        bool CapsLockActive = false;

        MouseCode Button = MouseCode::Button0;
        int MouseX = 0;
        int MouseY = 0;
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
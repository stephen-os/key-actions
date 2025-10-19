#include "Recording.h"

#include "Lumina/Core/Input.h"

#include <sstream>
#include <iomanip>

namespace Lumina
{
    std::string RecordedEvent::ToString() const
    {
        std::stringstream ss;

        // Format time as MM:SS.mmm
        int minutes = static_cast<int>(Time) / 60;
        int seconds = static_cast<int>(Time) % 60;
        int milliseconds = static_cast<int>((Time - static_cast<int>(Time)) * 1000);

        ss << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setw(2) << seconds << "."
            << std::setw(3) << milliseconds << " | ";

        switch (Action)
        {
        case RecordedAction::KeyPressed:
            ss << "Key Pressed: " << Input::KeyCodeToString(Key);
            break;
        case RecordedAction::KeyReleased:
            ss << "Key Released: " << Input::KeyCodeToString(Key);
            break;
        case RecordedAction::MousePressed:
            ss << "Mouse Pressed: Button " << static_cast<int>(Button)
                << " at (" << MouseX << ", " << MouseY << ")";
            break;
        case RecordedAction::MouseReleased:
            ss << "Mouse Released: Button " << static_cast<int>(Button)
                << " at (" << MouseX << ", " << MouseY << ")";
            break;
        case RecordedAction::MouseMoved:
            ss << "Mouse Moved to (" << MouseX << ", " << MouseY << ")";
            break;
        case RecordedAction::MouseScrolled:
            ss << "Mouse Scrolled: dx=" << ScrollDX << ", dy=" << ScrollDY;
            break;
        }

        return ss.str();
    }
}
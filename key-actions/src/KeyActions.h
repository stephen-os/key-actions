#include "Lumina/Core/Layer.h"

#include "Lumina/Core/Events/KeyEvent.h"
#include "Lumina/Core/Application.h"
#include "Lumina/Core/KeyCode.h"

#include "Lumina/Core/Log.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace Lumina
{
    struct KeyRecord
    {
        int KeyCode;
        float PressTime;
        float ReleaseTime;
        float Duration;

        std::string GetKeyName() const
        {
            Key key = static_cast<Lumina::KeyCode>(KeyCode);

            // Printable characters
            if (KeyCode >= 32 && KeyCode <= 126)
                return std::string(1, (char)KeyCode);

            // Function keys
            if (key >= KeyCode::F1 && key <= KeyCode::F25)
            {
                int fNum = KeyCode - (int)KeyCode::F1 + 1;
                return "F" + std::to_string(fNum);
            }

            // Keypad numbers
            if (key >= KeyCode::KP0 && key <= KeyCode::KP9)
            {
                int kpNum = KeyCode - (int)KeyCode::KP0;
                return "KP" + std::to_string(kpNum);
            }

            // Special keys
            switch (key)
            {
            case KeyCode::Space: return "SPACE";
            case KeyCode::Apostrophe: return "APOSTROPHE";
            case KeyCode::Comma: return "COMMA";
            case KeyCode::Minus: return "MINUS";
            case KeyCode::Period: return "PERIOD";
            case KeyCode::Slash: return "SLASH";
            case KeyCode::Semicolon: return "SEMICOLON";
            case KeyCode::Equal: return "EQUAL";
            case KeyCode::LeftBracket: return "LEFT_BRACKET";
            case KeyCode::Backslash: return "BACKSLASH";
            case KeyCode::RightBracket: return "RIGHT_BRACKET";
            case KeyCode::GraveAccent: return "GRAVE_ACCENT";
            case KeyCode::Escape: return "ESCAPE";
            case KeyCode::Enter: return "ENTER";
            case KeyCode::Tab: return "TAB";
            case KeyCode::Backspace: return "BACKSPACE";
            case KeyCode::Insert: return "INSERT";
            case KeyCode::Delete: return "DELETE";
            case KeyCode::Right: return "RIGHT";
            case KeyCode::Left: return "LEFT";
            case KeyCode::Down: return "DOWN";
            case KeyCode::Up: return "UP";
            case KeyCode::PageUp: return "PAGE_UP";
            case KeyCode::PageDown: return "PAGE_DOWN";
            case KeyCode::Home: return "HOME";
            case KeyCode::End: return "END";
            case KeyCode::CapsLock: return "CAPS_LOCK";
            case KeyCode::ScrollLock: return "SCROLL_LOCK";
            case KeyCode::NumLock: return "NUM_LOCK";
            case KeyCode::PrintScreen: return "PRINT_SCREEN";
            case KeyCode::Pause: return "PAUSE";
            case KeyCode::KPDecimal: return "KP_DECIMAL";
            case KeyCode::KPDivide: return "KP_DIVIDE";
            case KeyCode::KPMultiply: return "KP_MULTIPLY";
            case KeyCode::KPSubtract: return "KP_SUBTRACT";
            case KeyCode::KPAdd: return "KP_ADD";
            case KeyCode::KPEnter: return "KP_ENTER";
            case KeyCode::KPEqual: return "KP_EQUAL";
            case KeyCode::LeftShift: return "LEFT_SHIFT";
            case KeyCode::LeftControl: return "LEFT_CONTROL";
            case KeyCode::LeftAlt: return "LEFT_ALT";
            case KeyCode::LeftSuper: return "LEFT_SUPER";
            case KeyCode::RightShift: return "RIGHT_SHIFT";
            case KeyCode::RightControl: return "RIGHT_CONTROL";
            case KeyCode::RightAlt: return "RIGHT_ALT";
            case KeyCode::RightSuper: return "RIGHT_SUPER";
            case KeyCode::Menu: return "MENU";
            default: return "UNKNOWN_" + std::to_string(KeyCode);
            }
        }
    };

    class KeyActions : public Layer
    {
    public:
        KeyActions() : Layer("KeyActions") {}

        virtual void OnAttach() override
        {
            DockWindowSplit("Key Recording", DockPosition::Center);
            LUMINA_LOG_INFO("KeyActions layer attached");
        }

        virtual void OnDetach() override
        {
            LUMINA_LOG_INFO("KeyActions layer detached");
        }

        virtual void OnEvent(Event& e) override
        {
            EventDispatcher dispatcher(e);
            dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& event) {
                return OnKeyPressed(event);
                });
            dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& event) {
                return OnKeyReleased(event);
                });
        }

        virtual void OnUpdate(float timestep) override
        {
            // Update could be used for additional processing if needed
        }

        virtual void OnUIRender() override
        {
            ImGui::Begin("Key Recording");

            ImGui::Text("Currently Pressed Keys:");
            ImGui::Separator();

            float currentTime = Application::GetTime();
            for (const auto& [keyCode, pressTime] : m_ActiveKeys)
            {
                float duration = currentTime - pressTime;
                KeyRecord temp;
                temp.KeyCode = keyCode;
                ImGui::Text("%s - Held for: %.3f seconds",
                    temp.GetKeyName().c_str(), duration);
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Text("Key History:");
            ImGui::Separator();

            // Show last 20 key records
            int displayCount = std::min(20, (int)m_KeyHistory.size());
            for (int i = m_KeyHistory.size() - 1; i >= m_KeyHistory.size() - displayCount && i >= 0; i--)
            {
                const auto& record = m_KeyHistory[i];
                ImGui::Text("[%.2fs] %s - Pressed: %.3fs, Released: %.3fs, Duration: %.3fs",
                    record.PressTime,
                    record.GetKeyName().c_str(),
                    record.PressTime,
                    record.ReleaseTime,
                    record.Duration);
            }

            if (ImGui::Button("Clear History"))
            {
                m_KeyHistory.clear();
            }

            ImGui::End();
        }

    private:
        bool OnKeyPressed(KeyPressedEvent& e)
        {
            // Ignore repeat events - we only care about the initial press
            if (e.IsRepeat())
                return false;

            int keyCode = e.GetKeyCode();
            float currentTime = Application::GetTime();

            // Record the press time
            m_ActiveKeys[keyCode] = currentTime;

            KeyRecord temp;
            temp.KeyCode = keyCode;
            LUMINA_LOG_INFO("Key Pressed: {0} at time {1}", temp.GetKeyName(), currentTime);

            return false; // Don't block the event
        }

        bool OnKeyReleased(KeyReleasedEvent& e)
        {
            int keyCode = e.GetKeyCode();
            float currentTime = Application::GetTime();

            // Find the press time
            auto it = m_ActiveKeys.find(keyCode);
            if (it != m_ActiveKeys.end())
            {
                KeyRecord record;
                record.KeyCode = keyCode;
                record.PressTime = it->second;
                record.ReleaseTime = currentTime;
                record.Duration = record.ReleaseTime - record.PressTime;

                m_KeyHistory.push_back(record);

                LUMINA_LOG_INFO("Key Released: {0} at time {1}, Duration: {2}s",
                    record.GetKeyName(), currentTime, record.Duration);

                // Remove from active keys
                m_ActiveKeys.erase(it);
            }

            return false; // Don't block the event
        }

    private:
        // Map of currently pressed keys (keycode -> press time)
        std::unordered_map<int, float> m_ActiveKeys;

        // History of all key presses and releases
        std::vector<KeyRecord> m_KeyHistory;
    };
}
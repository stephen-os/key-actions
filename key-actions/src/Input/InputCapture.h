#pragma once
#include "Core/Recording.h"
#include <functional>
#include <memory>

namespace Lumina
{
    class InputCapture
    {
    public:
        using KeyCallback = std::function<void(KeyCode key, bool pressed)>;
        using MouseButtonCallback = std::function<void(MouseCode button, bool pressed, int x, int y)>;
        using MouseMoveCallback = std::function<void(int x, int y)>;
        using MouseScrollCallback = std::function<void(int dx, int dy)>;

        virtual ~InputCapture() = default;

        // Lifecycle
        virtual bool Start() = 0;
        virtual void Stop() = 0;
        virtual bool IsActive() const = 0;

        // Callbacks
        void SetKeyCallback(KeyCallback callback) { m_KeyCallback = callback; }
        void SetMouseButtonCallback(MouseButtonCallback callback) { m_MouseButtonCallback = callback; }
        void SetMouseMoveCallback(MouseMoveCallback callback) { m_MouseMoveCallback = callback; }
        void SetMouseScrollCallback(MouseScrollCallback callback) { m_ScrollCallback = callback; }

        // Factory
        static std::unique_ptr<InputCapture> Create();

    protected:
        KeyCallback m_KeyCallback;
        MouseButtonCallback m_MouseButtonCallback;
        MouseMoveCallback m_MouseMoveCallback;
        MouseScrollCallback m_ScrollCallback;
    };
}
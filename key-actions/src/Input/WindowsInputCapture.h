#pragma once
#include "InputCapture.h"

#ifdef _WIN32

#include <Windows.h>
#include <mutex>

namespace Lumina
{
    class WindowsInputCapture : public InputCapture
    {
    public:
        WindowsInputCapture();
        ~WindowsInputCapture() override;

        bool Start() override;
        void Stop() override;
        bool IsActive() const override { return m_IsActive; }

    private:
        // Win32 hook callbacks (must be static)
        static LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);

        // Conversion utilities
        static KeyCode VirtualKeyToKeyCode(DWORD vkCode);
        static MouseCode ButtonToMouseCode(WPARAM wParam);

        // Thread safety
        std::mutex m_CallbackMutex;

        // Hook handles
        HHOOK m_KeyboardHook = nullptr;
        HHOOK m_MouseHook = nullptr;
        bool m_IsActive = false;

        // Singleton instance for callbacks
        static WindowsInputCapture* s_Instance;
    };
}

#endif // _WIN32
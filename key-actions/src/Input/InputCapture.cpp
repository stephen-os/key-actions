#include "InputCapture.h"

#ifdef _WIN32
#include "WindowsInputCapture.h"
#endif

namespace Lumina
{
    std::unique_ptr<InputCapture> InputCapture::Create()
    {
#ifdef _WIN32
        return std::make_unique<WindowsInputCapture>();
#else
        return nullptr;
#endif
    }
}
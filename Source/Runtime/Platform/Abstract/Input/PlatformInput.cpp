#include "PlatformInput.h"

std::unique_ptr<RawInputDevice> PlatformInput::CreateRawInputDevice(Window& window)
{
    return std::make_unique<RawInputDevice>(window);
}
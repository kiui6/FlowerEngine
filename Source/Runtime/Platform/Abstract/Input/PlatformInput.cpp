#include "PlatformInput.h"

std::unique_ptr<RawInputDevice> PlatformInput::CreateRawInputDevice()
{
    return std::unique_ptr<RawInputDevice>();
}
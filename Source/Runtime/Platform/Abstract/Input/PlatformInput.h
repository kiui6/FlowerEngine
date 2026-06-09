#pragma once

#include "RawInputDevice.h"

class PlatformInput
{
public:
    PlatformInput() = default;
    virtual ~PlatformInput() = default;

    std::unique_ptr<RawInputDevice> CreateRawInputDevice();
};
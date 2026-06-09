#pragma once

#include <string_view>

#include "Window.h"

class PlatformWindow
{
public:
    PlatformWindow() = default;
    virtual ~PlatformWindow() = default;

    Window CreateWindow();
};
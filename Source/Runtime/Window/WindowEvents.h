#pragma once
#include <cstdint>
#include <array>

enum class WindowEventType : uint8_t
{
    None,
    Resize,
    Minimize,
    Maximize,
    Hidden,
    Exposed,
    Fullscreen,
};

struct WindowEvent { 
    WindowEventType type = WindowEventType::None; 
    std::array<uint64_t, 8> parameters{};
};
#pragma once

#include <cstdint>

enum class RenderAttachment : uint8_t {
    Depth,
    Albedo,
    Relief,
    Final,
    MAX
};
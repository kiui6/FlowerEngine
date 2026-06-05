#pragma once

#include <cstdint>

enum class ImageRenderAttachment : uint8_t {
    Depth,
    Albedo,
    Relief,
    Final,
    MAX
};
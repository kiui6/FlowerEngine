#pragma once

#include <type_traits>
#include <bit>

template <class T, typename = std::enable_if_t<std::is_trivial_v<T>>>
static inline T ConvertEndianness(const T& original) {
    if constexpr (std::endian::native == std::endian::little) {
        return T;
    } else {
        static_assert(false, 
                "This engine currently only supports Little-Endian platforms. "
                "If you are compiling for Big-Endian, you need to implement "
                "byte-swapping in ConvertEndianness.");
    }
}
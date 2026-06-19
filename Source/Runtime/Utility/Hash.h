#pragma once

#include <cstdint>
#include <string>
#include <string_view>

constexpr uint64_t FNV1a64(std::string_view str, uint64_t hash = 14695981039346656037ULL) {
    for (unsigned char c : str) {
        hash ^= static_cast<uint8_t>(c);
        hash *= 1099511628211ULL;
    }
    return hash;
}

template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
struct GoldHash {
    constexpr uint64_t operator()(T x) const {
        using U = std::make_unsigned_t<T>;
        U val = static_cast<U>(x);
        if constexpr (sizeof(T) == 8) {
            val ^= val >> 33;
            return static_cast<uint64_t>(val * 0x9e3779b97f4a7c15ULL);
        } else if constexpr (sizeof(T) == 4) {
            val ^= val >> 16;
            return static_cast<uint64_t>(val * 0x9e3779b1U);
        } else if constexpr (sizeof(T) == 2) {
            val ^= val >> 8;
            return static_cast<uint64_t>(val * 0x9e37U);
        } else if constexpr (sizeof(T) == 1) {
            val ^= val >> 4;
            return static_cast<uint64_t>(val * 0x9eU);
        } else {
            static_assert(sizeof(T) <= 8, "Unsupported GoldHash integer type");
            return 0;
        }
    }
};

// Used for std::unordered_map as a hash function to allow heterogeneous lookup with std::string_view
// std::unordered_map<std::string, _KeyT, StringHash, std::equal_to<>>
struct StringHash {
    using is_transparent = void;

    std::size_t operator()(std::string_view sv) const noexcept {
        return FNV1a64(sv);
    }

    std::size_t operator()(const std::string& s) const noexcept {
        // explicitly convert std::string_view or else this function calls itself
        return operator()(std::string_view(s));
    }
};
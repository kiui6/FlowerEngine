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
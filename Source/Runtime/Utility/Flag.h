#pragma once

template<typename Enum>
constexpr bool HasFlag(Enum flags, Enum flag) {
    using Underlying = std::underlying_type_t<Enum>;
    return (static_cast<Underlying>(flags) & static_cast<Underlying>(flag)) != 0;
}
#pragma once

#include <type_traits>
#include <concepts>
#include <atomic>

template<typename T>
concept FlagCompatibleEnum = std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

template <FlagCompatibleEnum T>
class HasFlags
{
    using Underlying = std::underlying_type_t<T>;

    std::atomic<Underlying> m_flags{};
public:
    void SetFlag(T flag) {m_flags |= static_cast<Underlying>(flag);}
    void ClearFlag(T flag) {m_flags &= ~static_cast<Underlying>(flag);}
    void ClearAll() {m_flags = 0;}
    bool HasFlag(T flag) const {return m_flags & static_cast<Underlying>(flag);}
};
#pragma once

template <typename Tag, typename UnderlyingT>
class StrongID {
    static_assert(std::is_integral_v<UnderlyingT>, "StrongID requires an integral underlying type")
    static_assert(sizeof(StrongID) == sizeof(UnderlyingT), "StrongID size doesn't match Underlying's size");

    UnderlyingT m_value;
public:
    explicit constexpr StrongID(UnderlyingT value = 0) : m_value(value) {}
    constexpr UnderlyingT value() const { return m_value; }

    constexpr StrongID& operator=(const StrongID& other) noexcept { m_value = other.m_value; return *this; }

    constexpr bool operator==(const StrongID& other) const { return m_value == other.m_value; }
    constexpr bool operator!=(const StrongID& other) const { return m_value != other.m_value; }
    constexpr bool operator<(const StrongID& other) const { return m_value < other.m_value; }
    constexpr bool operator>(const StrongID& other) const { return m_value > other.m_value; }
    constexpr bool operator<=(const StrongID& other) const { return m_value <= other.m_value; }
    constexpr bool operator>=(const StrongID& other) const { return m_value >= other.m_value; }
};
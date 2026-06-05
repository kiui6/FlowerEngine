#pragma once

#include <concepts>
#include <type_traits>
#include <array>

template<typename T>
concept EnumClassArrayCompatibleType = 
    std::is_enum_v<T> 
    && !std::is_convertible_v<T, std::underlying_type_t<T>> 
    && requires {
        T::MAX;
    };

template <typename T, EnumClassArrayCompatibleType EnumT>
struct EnumClassArray {
    inline T& operator[](EnumT value) {return m_array[static_cast<UnderlyingT>(value)];}
    inline const T& operator[](EnumT value) const {return m_array[static_cast<UnderlyingT>(value)];}

    inline auto begin() {return m_array.begin();}
    inline auto end() {return m_array.end();}
protected:
    using UnderlyingT = std::underlying_type_t<EnumT>;

    std::array<T, (UnderlyingT)EnumT::MAX> m_array{}; 
};
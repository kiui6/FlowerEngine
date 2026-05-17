#pragma once

#include <concepts>
#include <string_view>
#include <string>
#include <span>

#include <Utility/ID.h>

#define FIELDID(id) MakeID32( #id )

class FieldBase
{
public:
    virtual void Serialize(){}
    virtual void Deserialize(){}
};

template<typename T>
concept FieldClass = std::is_base_of<FieldBase, T>::value;

template<typename T>
concept HasDecayType = requires {
    typename T::DecayType;
};

template<typename T>
concept FieldValueClass = 
    requires {
        typename T::DecayType;
        { T::Deserialize(std::span<const uint8_t>{}, std::declval<typename T::DecayType&>()) } 
            -> std::same_as<void>;
        { T::Serialize(std::declval<const typename T::DecayType&>(), std::declval<std::vector<uint8_t>&>()) } 
            -> std::same_as<void>;
    };
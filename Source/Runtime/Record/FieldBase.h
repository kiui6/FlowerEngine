#pragma once

#include <concepts>
#include <string_view>
#include <string>
#include <span>

#include <Utility/ID.h>
#include "RecordFieldMemory.h"

#define FIELDID(id) MakeID32( #id )

class RecordFieldMemory;

class FieldBase
{
public:
    virtual inline ID32 GetID() const = 0;
    virtual inline bool IsDirty() const = 0;
    virtual void Serialize(RecordFieldMemory& memory) = 0;
    virtual void Deserialize(RecordFieldMemory* memory) = 0;
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
        { T::Deserialize(std::declval<RecordFieldMemory*>(), std::declval<typename T::DecayType&>()) } 
            -> std::same_as<void>;
        { T::Serialize(std::declval<const typename T::DecayType&>(), std::declval<RecordFieldMemory&>()) } 
            -> std::same_as<void>;
    };
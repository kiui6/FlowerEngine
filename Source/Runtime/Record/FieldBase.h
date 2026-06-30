#pragma once

#include <concepts>
#include <string_view>
#include <string>
#include <span>

#include <Utility/ID.h>
#include "RecordIR/RecordObject.h"

#define FIELDID(id) MakeID32( #id )

class RecordFieldMemory;

class FieldBase
{
public:
    virtual inline ID32 GetID() const = 0;
    virtual inline bool IsDirty() const = 0;
    virtual void Serialize(RecordObject& builder) = 0;
    virtual void Deserialize(const FieldNode& node) = 0;
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
        { T::Serialize(std::declval<const typename T::DecayType&>(), std::declval<RecordFieldObject::NodeWrapper&>()) } 
            -> std::same_as<void>;
        { T::Deserialize(std::declval<const FieldNode&>(), std::declval<typename T::DecayType&>()) } 
            -> std::same_as<void>;
    };
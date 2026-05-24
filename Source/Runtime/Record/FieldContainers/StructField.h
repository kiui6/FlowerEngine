#pragma once

#include "../FieldBase.h"

#include <type_traits>
#include <concepts>

template<typename T>
concept IsPOD = std::is_trivial_v<T> && std::is_standard_layout_v<T>;

template <IsPOD T>
struct StructField
{
    using DecayType = T;

    static void Serialize(const DecayType& data, RecordFieldMemory& out) {}
    static void Deserialize(RecordFieldMemory* data, DecayType& out) {}
};
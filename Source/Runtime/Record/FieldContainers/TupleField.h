#pragma once

#include "../FieldBase.h"

#include <tuple>
#include <type_traits>
#include <concepts>

#include <Utility/Meta/IsTrivial.h>

struct TupleFieldBase {};

template <IsTrivial... Types>
struct TupleField : public TupleFieldBase {
    std::tuple<Types...> m_tuple;
public:
    template<size_t I> auto& get() { return std::get<I>(m_tuple); }
};

template <class TupleStruct, typename = std::enable_if<std::is_base_of_v<TupleFieldBase, TupleStruct>>>
struct FTuple
{
    using DecayType = TupleStruct;

    static void Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {}
    static void Deserialize(FieldNode* node, DecayType& out) {}
};
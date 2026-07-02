#pragma once

#include "../FieldBase.h"

#include <Log/Log.h>

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

    static bool Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {return false;}
    static bool Deserialize(const FieldNode& node, DecayType& out) {
        assert(node.type == FieldNodeType::Tuple);
        assert(node.data.children != nullptr);

        // TODO
        return false;
    }
};
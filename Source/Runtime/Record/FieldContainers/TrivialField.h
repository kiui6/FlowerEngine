#pragma once

#include "../FieldBase.h"

#include <Log/Log.h>

#include <Utility/Meta/IsTrivial.h>

#include <cstdint>

template <IsTrivial T, FieldNodeType NodeEnumT = FieldNodeType::Trivial>
struct FTrivial
{
    using DecayType = T;

    static bool Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {}
    static bool Deserialize(const FieldNode& node, DecayType& out) {
        if(node.type != NodeEnumT) {
            LOG(Assert, LogFTrivialDeserialize, "Passed node type mismatch!");
            return false;
        }

        memcpy(&out, &node.data, sizeof(T));

        return true;
    }
};

using FInteger = FTrivial<int32_t, FieldNodeType::Integer>;
using FLong = FTrivial<int64_t, FieldNodeType::Long>;

using FUnsigned = FTrivial<uint32_t, FieldNodeType::Unsigned>;

using FFloat = FTrivial<float, FieldNodeType::Float>;
using FDouble = FTrivial<double, FieldNodeType::Double>;

using FBool = FTrivial<bool, FieldNodeType::Bool>;
#pragma once

#include "../FieldBase.h"

#include <string_view>

struct StringField
{
    using DecayType = std::string;

    static void Serialize(const DecayType& data, RecordFieldMemory& out) {}
    static void Deserialize(RecordFieldMemory* data, DecayType& out) {
        out.reserve(data->size);
        out.assign(reinterpret_cast<const char*>(data->data.get()), data->size);
    }
};
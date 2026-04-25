#pragma once

#include <cstdint>

#include "FieldBase.h"
#include <Utility/Record.h>

#define FIELDID(id) CompMakeRecordType( #id )

template <FieldValueClass FieldValue>
class Field : public FieldBase
{
    uint32_t m_id;

    FieldValue::DecayType m_cached;
public:
    Field(uint32_t id) : m_id(id) {}
    Field(uint32_t id, FieldValue::DecayType defaultValue) : m_id(id), m_cached(defaultValue) {}

    FieldValue::DecayType& Get() { return m_cached; }

    void Serialize() override {
        std::vector<uint8_t> temp;
        FieldValue::Serialize(m_cached, temp);
    }

    void Deserialize() override {
        std::span<const uint8_t> temp;
        FieldValue::Deserialize(temp, m_cached);
    }
};
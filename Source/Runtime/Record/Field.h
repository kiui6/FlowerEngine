#pragma once

#include <cstdint>

#include "FieldBase.h"
#include <Utility/Record.h>

template <FieldContainerClass ValueType>
class Field : public FieldBase
{
    uint32_t m_id;
    ValueType m_value;
public:
    Field(uint32_t id) : m_id(id) {}
    Field(uint32_t id, ValueType defaultValue) : m_id(id), m_value(defaultValue) {}

};

#define DECLARE_FIELD(id, type, defaultValue) \
    Field<type> id = { #id , defaultValue};
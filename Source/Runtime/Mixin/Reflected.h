#pragma once

#include <concepts>

#include <Utility/ID.h>

class Reflected
{
protected:
    ID32 m_typeID;

    Reflected(ID32 type) : m_typeID(type) {}

    inline void SetType(ID32 type) {m_typeID = type;}
public:
    inline ID32 GetType() const { return m_typeID; }
};
#pragma once

#include <concepts>

#include <Utility/ID.h>

/*
 * Provides API for runtime type identification
 *
 * Consumer should call SetType in the constructor of each new derived class
 * Usually as Ctor() {SetType(StaticType());}
 */
class Typed
{
protected:
    ID32 m_typeID;

    Typed(ID32 type) : m_typeID(type) {}

    inline void SetType(ID32 type) {m_typeID = type;}
public:
    inline ID32 GetType() const { return m_typeID; }
};
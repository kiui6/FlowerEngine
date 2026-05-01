#pragma once

#include "../FieldBase.h"

struct Int8Field
{
    using DecayType = int8_t;
    
    static DecayType DefaultConstructor() {return 0;}

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out) {}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out) {}
};

struct Int16Field
{
    using DecayType = int16_t;

    static DecayType DefaultConstructor() {return 0;}

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out) {}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out) {}
};

struct Int32Field
{
    using DecayType = int32_t;

    static DecayType DefaultConstructor() {return 0;}

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out) {}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out) {}
};

struct UInt8Field
{
    using DecayType = uint8_t;

    static DecayType DefaultConstructor() {return 0;}

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out) {}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out) {}
};

struct UInt16Field
{
    using DecayType = uint16_t;

    static DecayType DefaultConstructor() {return 0;}

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out) {}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out) {}
};

struct UInt32Field
{
    using DecayType = uint32_t;

    static DecayType DefaultConstructor() {return 0;}

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out) {}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out) {}
};
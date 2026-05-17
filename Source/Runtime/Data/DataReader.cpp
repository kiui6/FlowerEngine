#include "DataReader.h"

std::optional<std::byte> DataReader::ReadByte(bool advance)
{
    return std::optional<std::byte>();
}

std::optional<std::byte *> DataReader::ReadBytes(size_t length, bool advance)
{
    return std::optional<std::byte *>();
}

std::optional<int16_t> DataReader::ReadInt16(bool advance)
{
    return std::optional<int16_t>();
}

std::optional<int32_t> DataReader::ReadInt32(bool advance)
{
    return std::optional<int32_t>();
}

std::optional<int64_t> DataReader::ReadInt64(bool advance)
{
    return std::optional<int64_t>();
}

std::optional<uint16_t> DataReader::ReadUInt16(bool advance)
{
    return std::optional<uint16_t>();
}

std::optional<uint32_t> DataReader::ReadUInt32(bool advance)
{
    return std::optional<uint32_t>();
}

std::optional<uint64_t> DataReader::ReadUInt64(bool advance)
{
    return std::optional<uint64_t>();
}

std::optional<const char *> DataReader::ReadString(size_t length, bool advance)
{
    return std::optional<const char *>();
}

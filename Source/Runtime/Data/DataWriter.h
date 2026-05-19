#pragma once

#include <memory>
#include <vector>
#include <cassert>

#include <Platform/FileBase.h>

class DataWriter {
    std::shared_ptr<FileBase> m_file = nullptr;
    std::vector<std::byte> m_buffer;
    size_t cursor = 0;
public:
    DataWriter() {}
    DataWriter(std::shared_ptr<FileBase> file) : m_file(file) {}

    inline void Advance(ptrdiff_t diff) {cursor += diff;}
    inline void SetOffset(size_t offset) {cursor = offset;}

    void WriteByte(std::byte val);
    void WriteBytes(std::byte* val, size_t size);

    void WriteInt16(int16_t val);
    void WriteInt32(int32_t val);
    void WriteInt64(int64_t val);

    void WriteUInt16(uint16_t val);
    void WriteUInt32(uint32_t val);
    void WriteUInt64(uint64_t val);

    void WriteString(const std::string& string);

};
#pragma once

#include <memory>
#include <vector>
#include <cassert>
#include <optional>
#include <type_traits>
#include <cstring>
#include <bit>

#include <Platform/Abstract/Filesystem/FileBase.h>

#include "DataBuffer.h"
#include "DataView.h"

class FileBuffer {
    std::shared_ptr<FileBase> m_file = nullptr;

    DataBuffer m_buffer;
public:
    FileBuffer() = default;
    FileBuffer(std::shared_ptr<FileBase> file) : m_file(file) {}
    ~FileBuffer() = default;

    DataBuffer& GetBuffer() {return m_buffer;}
    DataView GetView(size_t offset, size_t size) const {return m_buffer.MakeView(offset, size);}

};
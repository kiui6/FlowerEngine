#pragma once

#include <memory>
#include <cassert>

#include <Platform/Abstract/Filesystem/FileBase.h>

class DataView {
    friend class DataManager;

    std::shared_ptr<FileBase> m_file = nullptr;
    size_t m_offset = 0, m_size = 0;

    DataView() = default;
    DataView(std::shared_ptr<FileBase> file) : m_file(file), m_size(m_file ? m_file->GetSize() : 0) {}
    DataView(std::shared_ptr<FileBase> file, size_t offset) : m_file(file), m_offset(offset), m_size(m_file ? m_file->GetSize()-offset : 0) {}
    DataView(std::shared_ptr<FileBase> file, size_t offset, size_t size) : m_file(file), m_offset(offset), m_size(size) {}
public:

    const std::byte* begin()    const { return m_file->GetData() + m_offset; }
    const std::byte* end()      const { return begin() + m_size; }
    size_t size()               const { return m_size; }
    const std::byte* data()     const { return begin(); }
    bool empty()                const { return m_size == 0; }

    DataView MakeSubView(size_t offset, size_t size) const noexcept {
        return DataView(m_file, m_offset + offset, size);
    } 

    const std::byte& operator [](size_t idx) const noexcept {
        assert(idx < m_size && "DataView index out of range");
        return *(begin() + idx);
    }

    explicit operator bool() const { return m_file.get() != nullptr; }
};
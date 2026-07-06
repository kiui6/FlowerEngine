#pragma once

#include <memory>
#include <cassert>

#include "DataView.h"

/*
 * Read-write view for data inside a resizable buffer container 
 */
class DataBuffer {
protected:
    std::byte* m_data = nullptr;
    size_t m_offset = 0, m_size = 0;
public:
    DataBuffer() = default;
    DataBuffer(std::byte* data, size_t size, size_t offset = 0) : m_data(data), m_size(size), m_offset(offset) {}
    // Copy constructor
    DataBuffer(const DataBuffer& right) {
        m_data = right.m_data;
        m_offset = right.m_offset;
        m_size = right.m_size;
    }

    // Move constructor
    DataBuffer(DataBuffer && right) {
        m_data = std::move(right.m_data);
        m_offset = std::exchange(right.m_offset, 0);
        m_size = std::exchange(right.m_size, 0);
    }
    ~DataBuffer() = default;

    const std::byte* begin()    const { return m_data + m_offset; }
    const std::byte* end()      const { return begin() + m_size; }
    size_t size()               const { return m_size; }
    const std::byte* data()     const { return begin(); }
    bool empty()                const { return m_size == 0; }

    DataView MakeView(size_t offset, size_t size) const noexcept {
        return DataView(m_data, size, m_offset + offset);
    } 

    // Copy assign
    DataBuffer& operator =(const DataBuffer& right) {
        m_data = right.m_data;
        m_offset = right.m_offset;
        m_size = right.m_size;
        return *this;
    }

    // Move assign
    DataBuffer& operator =(DataBuffer && right) {
        m_data = std::move(right.m_data);
        m_offset = std::exchange(right.m_offset, 0);
        m_size = std::exchange(right.m_size, 0);
        return *this;
    }

    const std::byte& operator [](size_t idx) const noexcept {
        assert(idx < m_size && "DataBuffer index out of range");
        return *(begin() + idx);
    }

    operator bool() const { return m_data != nullptr; }
};
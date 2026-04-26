#pragma once

#include <memory>
#include <vector>
#include <cassert>

constexpr size_t MAX_ARENA_BLOCKS = 256;

template <size_t BlockSize>
class ArenaAllocator
{
    std::vector<std::unique_ptr<std::byte[]>> m_memBlocks;
    size_t m_lastBlock = 0;
    size_t m_currentOffset = 0;

public:
    ArenaAllocator() {
        CreateNewBlock();
        m_lastBlock = 0;
        m_currentOffset = 0;
    }

    ArenaAllocator(const Arena&) = delete;
    ArenaAllocator(Arena&&) = delete;

    void* Allocate(size_t size, size_t align = alignof(std::max_align_t)) {
        uintptr_t addr = reinterpret_cast<uintptr_t>(m_memBlocks[m_lastBlock].get()) + m_currentOffset;
        size_t pad = (align - (addr % align)) % align;

        // Return nullptr if requested size is more than block size
        if(pad + size > BlockSize) {
            assert(!"Allocation too large");
            return nullptr;
        }

        // Advance or create a new block before proceeding if requested memory doesn't fit current block
        if(m_currentOffset + pad + size > BlockSize) {
            // If last block in use is the last block available, create new block
            if(m_lastBlock == m_memBlocks.size() - 1) {
                CreateNewBlock();
            } 
                
            m_lastBlock++;
            m_currentOffset = 0;

            addr = reinterpret_cast<uintptr_t>(m_memBlocks[m_lastBlock].get()) + m_currentOffset;
            pad = (align - (addr % align)) % align;
        }

        m_currentOffset += pad + size;

        return reinterpret_cast<void*>(addr + pad);
    }

    template<typename T>
    T* AllocateObject() {
        void* memPtr = Allocate(sizeof(T), alignof(T));
        T* objectPtr = new (memPtr) T();
        return objectPtr;
    }

    void Reset() {
        m_lastBlock = 0;
        m_currentOffset = 0;
    }

private:
    void CreateNewBlock() {
        m_memBlocks.emplace_back(std::make_unique<std::byte[]>(BlockSize));
    }
};
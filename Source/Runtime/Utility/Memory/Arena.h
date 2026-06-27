#pragma once

#include <memory>
#include <vector>
#include <cassert>


// TODO
/* 
 * An arena that has no fall back
 * Once arena is exhausted subsequent allocations will fail
 * Once failed, assertion is thrown and application is terminated 
 *  
 * BlockSize should be chosen to fit 100% of cases.
 * This structure should be used if compile time guarantees are present
 */
template <size_t BlockSize>
class StaticArena
{
    std::unique_ptr<std::byte[]> m_memory;
    size_t m_currentOffset = 0;
public:
    StaticArena() {
        m_memory = std::make_unique<std::byte[]>(BlockSize);
        m_currentOffset = 0;
    }

    StaticArena(const StaticArena&) = delete;
    StaticArena(StaticArena&& right) {
        m_memory = std::move(right.m_memory);
        m_currentOffset = std::exchange(right.m_currentOffset, 0);
    }

    void* Allocate(size_t size, size_t align = alignof(std::max_align_t)) {
        uintptr_t addr = reinterpret_cast<uintptr_t>(m_memory.get()) + m_currentOffset;
        size_t pad = (align - (addr % align)) % align;

        if((pad + size) > (BlockSize - m_currentOffset)) [[unlikely]] {
            assert(!"Allocation too large");
            return nullptr;
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
        m_currentOffset = 0;
    }
};

/* 
 * An arena that falls back to creating more contigious memory blocks
 * Once latest block is exhausted it allocates a new block. 
 * 
 * After new allocations, cache efficiency drops near the seams
 * BlockSize should be chosen to fit 80-90% of cases, with occasional fall backs to new blocks
 */
template <size_t BlockSize>
class GrowingArena
{
    std::vector<std::unique_ptr<std::byte[]>> m_memBlocks;
    size_t m_lastBlock = 0;
    size_t m_currentOffset = 0;

public:
    GrowingArena() {
        CreateNewBlock();
        m_lastBlock = 0;
        m_currentOffset = 0;
    }

    GrowingArena(const GrowingArena&) = delete;
    GrowingArena(GrowingArena&& right) {
        m_memBlocks = std::move(right.m_memBlocks);
        m_lastBlock = std::exchange(right.m_lastBlock, 0);
        m_currentOffset = std::exchange(right.m_currentOffset, 0);
    }

    void* Allocate(size_t size, size_t align = alignof(std::max_align_t)) {
        uintptr_t addr = reinterpret_cast<uintptr_t>(m_memBlocks[m_lastBlock].get()) + m_currentOffset;
        size_t pad = (align - (addr % align)) % align;

        // Return nullptr if requested size is more than block size
        if(pad + size > BlockSize) [[unlikely]] {
            assert(!"Allocation too large");
            return nullptr;
        }

        // Advance or create a new block before proceeding if requested memory doesn't fit current block
        if(m_currentOffset + pad + size > BlockSize) [[unlikely]] {
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

    inline bool IsEmpty() const { return !(m_currentOffset || m_lastBlock);}

private:
    void CreateNewBlock() {
        m_memBlocks.emplace_back(std::make_unique<std::byte[]>(BlockSize));
    }
};
#pragma once

#include "../SIMD.h"

#include <cassert>
#include <bit>

template <class K, class V, class HashFunction = std::hash<K>, class _KeyEq = std::equal_to<K>, class _Allocator = std::allocator<std::pair<const K, V>>>
class FlatHashMap {
    using Slot = std::pair<const K, V>;

    struct Group {
        uint8_t bytes[16];
    };

    struct Iterator {
    private:
        uint8_t* m_metadata;
        Slot* m_slots;
        size_t m_capacity;
        size_t m_index;

        void SeekValidSlot() {
            while (m_metadata[m_index] > 0x7F && m_index < m_capacity) {
                ++m_index;
            }
        }

    public:
        Iterator(uint8_t* metadata, Slot* slots, size_t capacity, size_t index)
            : m_metadata(metadata), m_slots(slots), m_capacity(capacity), m_index(index) {
            SeekValidSlot();
        }

        Iterator& operator++() {
            ++m_index;
            SeekValidSlot();
            return *this;
        }

        Slot& operator*() {
            return m_slots[m_index];
        }
        
        const Slot& operator*() const {
            return m_slots[m_index];
        }

        Slot* operator->() {
            return &m_slots[m_index];
        }

        const Slot* operator->() const {
            return &m_slots[m_index];
        }

        bool operator==(const Iterator& other) const {
            return m_index == other.m_index && m_slots == other.m_slots;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    using MetaAllocator = std::allocator_traits<_Allocator>
    :: template rebind_alloc<uint8_t>;
    
    _Allocator m_alloc{};
    size_t m_occupied, m_capacity;
    Slot* m_slots;
    uint8_t* m_metadata;
public:
    FlatHashMap()
    : m_occupied(0), m_capacity(16)
    {
        MetaAllocator metaAlloc(m_alloc);
        m_slots = std::allocator_traits<_Allocator>::allocate(m_alloc, m_capacity);
        m_metadata = std::allocator_traits<MetaAllocator>::allocate(metaAlloc, m_capacity + 16);
        memset(m_metadata, 0x80, m_capacity + 16);
    }

    FlatHashMap(const _Allocator & alloc)
    : m_occupied(0), m_capacity(16), m_alloc(alloc)
    {
        MetaAllocator metaAlloc(m_alloc);
        m_slots = std::allocator_traits<_Allocator>::allocate(m_alloc, m_capacity);
        m_metadata = std::allocator_traits<MetaAllocator>::allocate(metaAlloc, m_capacity + 16);
        memset(m_metadata, 0x80, m_capacity + 16);
    }

    FlatHashMap(size_t initCapacity)
    : m_occupied(0), m_capacity(initCapacity)
    {
        assert(initCapacity % 16 == 0 && "Initial capacity must be divisible by 16");

        MetaAllocator metaAlloc(m_alloc);
        m_slots = std::allocator_traits<_Allocator>::allocate(m_alloc, m_capacity);
        m_metadata = std::allocator_traits<MetaAllocator>::allocate(metaAlloc, m_capacity + 16);
        memset(m_metadata, 0x80, m_capacity + 16);
    }

    FlatHashMap(size_t initCapacity, const _Allocator & alloc)
    : m_occupied(0), m_capacity(initCapacity), m_alloc(alloc)
    {
        assert(initCapacity % 16 == 0 && "Initial capacity must be divisible by 16");

        MetaAllocator metaAlloc(m_alloc);
        m_slots = std::allocator_traits<_Allocator>::allocate(m_alloc, m_capacity);
        m_metadata = std::allocator_traits<MetaAllocator>::allocate(metaAlloc, m_capacity + 16);
        memset(m_metadata, 0x80, m_capacity + 16);
    }

    FlatHashMap(_Allocator && alloc)
    : m_occupied(0), m_capacity(16), m_alloc(alloc)
    {
        MetaAllocator metaAlloc(m_alloc);
        m_slots = std::allocator_traits<_Allocator>::allocate(m_alloc, m_capacity);
        m_metadata = std::allocator_traits<MetaAllocator>::allocate(metaAlloc, m_capacity + 16);
        memset(m_metadata, 0x80, m_capacity + 16);
    }

    ~FlatHashMap() {
        if(m_metadata && m_slots ) {
            for(int i = 0; i < m_occupied; --i) {
                if (m_metadata[i] != 0x80 && m_metadata[i] != 0xFE) {
                    m_slots[i].~Slot();
                }
            }
        }

        if(m_slots) {
            std::allocator_traits<_Allocator>::deallocate(m_alloc, m_slots, m_capacity);
        }

        if(m_metadata) {
            MetaAllocator metaAlloc(m_alloc);
            std::allocator_traits<MetaAllocator>::deallocate(metaAlloc, m_metadata, m_capacity + 16);
        }
    }

    FlatHashMap<K, V, HashFunction, _KeyEq, _Allocator>& operator=(const FlatHashMap<K, V, HashFunction, _KeyEq, _Allocator>& other) {
        // TODO: Copy
        assert(!"Unimplemented");
        return *this;
    }

    FlatHashMap<K, V, HashFunction, _KeyEq, _Allocator>& operator=(FlatHashMap<K, V, HashFunction, _KeyEq, _Allocator> && other) {
        m_alloc = other.m_alloc;
        m_capacity = other.m_capacity;
        m_metadata = other.m_metadata;
        m_occupied = other.m_occupied;
        m_slots = other.m_slots;

        other.m_capacity = 0;
        other.m_metadata = nullptr;
        other.m_occupied = 0;
        other.m_slots = nullptr;

        return *this;
    }

    inline Slot* insert(const K&  key, const V& value) {
        return emplace(key, value);
    }

    template <class _VTy>
    inline Slot* emplace(const K&  key, _VTy && value) {
        if(m_occupied >= (m_capacity * 7) * 0.125f) {
            _Rehash(m_capacity * 2);
        }

        return _Emplace(key, std::forward<_VTy>(value));
    }

    inline Slot* find(const K& key) {
        const uint64_t hash = HashFunction{}(key);
        const uint8_t h2 = hash & 0x7F;
        size_t idx = ((hash >> 7) & (m_capacity - 1)) & ~15;

        const simd128i vecH2 = simdSet128i((char)h2);
        const simd128i vecEmpty = simdSet128i((char)0x80);

        while(true) {
            simd128i metadata = simdLoad128i((const simd128i*)&m_metadata[idx]);
            simd128i cmp = simdCmpEq8(metadata, vecH2);
            uint16_t mask = simdMovemask8(cmp);

            while(mask) {
                uint16_t bit = std::countr_zero(mask);
                size_t slot_idx = (idx + bit) & (m_capacity - 1);

                if (m_slots[slot_idx].first == key) {
                    return &m_slots[slot_idx];
                }

                mask &= mask - 1;
            }

            simd128i cmpEmpty = simdCmpEq8(metadata, vecEmpty);
            int emptyMask = simdMovemask8(cmpEmpty);
            
            if (!emptyMask) {
                idx = (idx + 16) & (m_capacity - 1);
            } else {
                return nullptr;
            }
        }

        return nullptr;
    }

    void erase(const K& key) {
        Slot* slot = find(key);
        if (!slot) return;

        size_t idx = slot - m_slots;
        slot->~Slot();
        m_metadata[idx] = 0xFE;
        m_occupied--;
    }

    V* operator[](const K& key) { return find(key); }
    const V* operator[](const K& key) const { return find(key); }

    inline Iterator begin() {return Iterator(m_metadata, m_slots, m_capacity, 0);}
    inline Iterator end() {return Iterator(m_metadata, m_slots, m_capacity, m_capacity);}

    inline const Iterator begin() const {return Iterator(m_metadata, m_slots, m_capacity, 0);}
    inline const Iterator end() const {return Iterator(m_metadata, m_slots, m_capacity, m_capacity);}

    inline bool empty() {return m_occupied;}

protected:
    template <class _VTy>
    inline Slot* _Emplace(const K& key, _VTy && value) {
        const uint64_t hash = HashFunction{}(key);
        const uint8_t h2 = hash & 0x7F;
        size_t idx = ((hash >> 7) & (m_capacity - 1)) & ~15;

        const simd128i vecEmpty = simdSet128i((char)0x80);
        const simd128i vecDeleted = simdSet128i((char)0xFE);
        const simd128i vecH2 = simdSet128i((char)h2);

        while (true) {
            const simd128i metadata = simdLoad128i((const simd128i*)&m_metadata[idx]);

            const simd128i cmp = simdCmpEq8(metadata, vecH2);
            uint16_t matchMask = simdMovemask8(cmp);

            const simd128i cmpEmpty = simdCmpEq8(metadata, vecEmpty);
            uint16_t emptyMask = simdMovemask8(cmpEmpty);
            if (emptyMask) {
                matchMask &= (emptyMask - 1);
            }

            while (matchMask) {
                const uint16_t bit = std::countr_zero(matchMask);
                const size_t slot_idx = (idx + bit) & (m_capacity - 1);
                if (m_slots[slot_idx].first == key) {
                    m_slots[slot_idx].second = std::forward<_VTy>(value);
                    return &m_slots[slot_idx];
                }
                matchMask &= matchMask - 1;
            }

            const simd128i cmpDeleted = simdCmpEq8(metadata, vecDeleted);
            const uint16_t deletedMask = simdMovemask8(cmpDeleted);

            const uint16_t availableMask = emptyMask | deletedMask;

            if (availableMask) {
                const uint16_t bit = std::countr_zero(availableMask);
                const size_t slot_idx = (idx + bit) & (m_capacity - 1);

                new (&m_slots[slot_idx]) Slot{key, std::forward<_VTy>(value)};
                m_metadata[slot_idx] = h2;

                m_occupied++;
                return &m_slots[slot_idx];
            }

            idx = (idx + 16) & (m_capacity - 1);
        }

        return nullptr;
    }

    void _Rehash(size_t newCapacity) {
        Slot* oldSlots = m_slots;
        uint8_t* oldMetadata = m_metadata;
        size_t oldCapacity = m_capacity;

        m_occupied = 0;
        m_capacity = newCapacity;
        
        MetaAllocator metaAlloc(m_alloc);
        m_slots = std::allocator_traits<_Allocator>::allocate(m_alloc, m_capacity);
        m_metadata = std::allocator_traits<MetaAllocator>::allocate(metaAlloc, m_capacity + 16);

        memset(m_metadata, 0x80, m_capacity + 16);

        for (size_t i = 0; i < oldCapacity; ++i) {
            if (oldMetadata[i] != 0x80 && oldMetadata[i] != 0xFE) {
                _Emplace(oldSlots[i].first, std::move(oldSlots[i].second));
                oldSlots[i].~Slot();
            }
        }

        std::allocator_traits<_Allocator>::deallocate(m_alloc, oldSlots, oldCapacity);
        std::allocator_traits<MetaAllocator>::deallocate(metaAlloc, oldMetadata, oldCapacity + 16);
    }
};
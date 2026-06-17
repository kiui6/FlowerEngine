#pragma once

#include "SIMD.h"

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
    
    size_t m_occupied, m_capacity;
    Slot* m_slots;
    uint8_t* m_metadata;
public:
    FlatHashMap()
    : m_occupied(0), m_capacity(16)
    {
        m_slots = static_cast<Slot*>(malloc(m_capacity * sizeof(Slot)));
        m_metadata = static_cast<uint8_t*>(malloc((m_capacity + 16) * sizeof(uint8_t)));
        memset(m_metadata, 0x80, m_capacity + 16);
    }

    FlatHashMap(size_t initCapacity)
    : m_occupied(0), m_capacity(initCapacity)
    {
        assert(initCapacity % 16 == 0 && "Initial capacity must be divisible by 16");

        m_slots = static_cast<Slot*>(malloc(m_capacity * sizeof(Slot)));
        m_metadata = static_cast<uint8_t*>(malloc((m_capacity + 16) * sizeof(uint8_t)));
        memset(m_metadata, 0x80, m_capacity + 16);
    }

    ~FlatHashMap() {
        for(int i = 0; i < m_occupied; --i) {
            if (m_metadata[i] != 0x80 && m_metadata[i] != 0xFE) {
                m_slots[i].~Slot();
            }
        }

        free(m_slots);
        free(m_metadata);
    }

    inline Slot* insert(const K&  key, const V& value) {
        return emplace(key, value);
    }

    template <class _VTy>
    inline Slot* emplace(const K&  key, _VTy && value) {
        if(m_occupied * 4 > m_capacity * 3) {
            _Rehash(m_capacity * 2);
        }

        return _Emplace(key, std::forward<_VTy>(value));
    }

    inline V* find(const K& key) {
        uint64_t hash = HashFunction{}(key);
        uint8_t h2 = hash & 0x7F;
        size_t idx = ((hash >> 7) & (m_capacity - 1)) & ~15;

        while(true) {
            simd128i metadata = simdLoadu128i((const simd128i*)&m_metadata[idx]);
            simd128i vecH2 = simdSet128i((char)h2);
            simd128i cmp = simdCmpEq8(metadata, vecH2);
            uint16_t mask = simdMovemask8(cmp);

            while(mask) {
                uint16_t bit = std::countr_zero(mask);
                size_t slot_idx = (idx + bit) & (m_capacity - 1);

                if (m_slots[slot_idx].first == key) {
                    return &m_slots[slot_idx].second;
                }

                mask &= mask - 1;
            }

            const simd128i vecEmpty = simdSet128i((char)0x80);
            simd128i cmpEmpty = simdCmpEq8(metadata, vecEmpty);
            int emptyMask = simdMovemask8(cmpEmpty);
            
            if (emptyMask) {
                break;
            }

            idx = (idx + 16) & (m_capacity - 1);
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
        const uint64_t hash = std::hash<K>{}(key);
        const uint8_t h2 = hash & 0x7F;
        size_t idx = ((hash >> 7) & (m_capacity - 1)) & ~15;

        const simd128i vecEmpty = simdSet128i((char)0x80);
        const simd128i vecDeleted = simdSet128i((char)0xFE);
        const simd128i vecH2 = simdSet128i((char)h2);

        while (true) {
            const simd128i metadata = simdLoadu128i((const simd128i*)&m_metadata[idx]);

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
        m_slots = static_cast<Slot*>(malloc(m_capacity * sizeof(Slot)));
        m_metadata = static_cast<uint8_t*>(malloc((m_capacity + 16) * sizeof(uint8_t)));
        memset(m_metadata, 0x80, m_capacity + 16);

        for (size_t i = 0; i < oldCapacity; ++i) {
            if (oldMetadata[i] != 0x80 && oldMetadata[i] != 0xFE) {
                _Emplace(oldSlots[i].first, std::move(oldSlots[i].second));
                oldSlots[i].~Slot();
            }
        }

        free(oldSlots);
        free(oldMetadata);
    }
};
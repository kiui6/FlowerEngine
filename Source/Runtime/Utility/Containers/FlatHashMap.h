#pragma once

#include "../SIMD.h"
#include "../Hash.h"

#include <cassert>
#include <bit>
#include <functional>

template <typename KeyType, typename = void>
struct FlatHashMapDefaultHash {
    using type = std::hash<KeyType>;
};

template <typename T>
struct FlatHashMapDefaultHash<T, std::enable_if_t<std::is_integral_v<T>>> {
    using type = GoldHash<T>;
};

template <typename T>
struct FlatHashMapDefaultHash<T, std::enable_if_t<std::is_base_of_v<std::string, T>>> {
    using type = StringHash;
};

template <class _Func, class _TArg>
struct _FlatHashMapHash : _Func {
    uint64_t operator()(const _TArg& key) const {return _Func::operator()(key);}
};

template <class K, class V, class HashFunction = FlatHashMapDefaultHash<K>::type, class _KeyEq = std::equal_to<>, class _Allocator = std::allocator<std::pair<const K, V>>>
class FlatHashMap : _Allocator, _FlatHashMapHash<HashFunction, K> {
    using Slot = std::pair<const K, V>;
    using ContainerType = FlatHashMap<K, V, HashFunction, _KeyEq, _Allocator>;

    struct Iterator {
    private:
        ContainerType* m_map;
        size_t m_index;

        void _SeekNextValidSlot() {
            while (m_map->m_metadata[m_index] > 0x7F && m_index < m_map->m_capacity) {
                ++m_index;
            }
        }

    public:
        Iterator(ContainerType* map, size_t index)
            : m_map(map), m_index(index)
        {
            _SeekNextValidSlot();
        }

        inline Iterator& operator++() {
            ++m_index;
            _SeekNextValidSlot();
            return *this;
        }

        inline Slot& operator*() {
            return m_map->m_slots[m_index];
        }
        
        inline const Slot& operator*() const {
            return m_map->m_slots[m_index];
        }

        inline Slot* operator->() {
            return &m_map->m_slots[m_index];
        }

        inline const Slot* operator->() const {
            return &m_map->m_slots[m_index];
        }

        inline bool operator==(const Iterator& other) const {
            return m_index == other.m_index && m_map == other.m_map;
        }

        inline bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    using MetaAllocator = std::allocator_traits<_Allocator>
    :: template rebind_alloc<uint8_t>;
    
    size_t m_occupied{}, m_capacity{};
    Slot* m_slots{};
    uint8_t* m_metadata{};
public:
    FlatHashMap()
    : m_occupied(0), m_capacity(16)
    {
        _Resize(m_capacity);
    }

    FlatHashMap(const _Allocator & alloc)
    : m_occupied(0), m_capacity(16), _Allocator(alloc)
    {
        _Resize(m_capacity);
    }

    FlatHashMap(size_t initCapacity)
    : m_occupied(0), m_capacity(initCapacity)
    {
        assert(initCapacity % 16 == 0 && "Initial capacity must be divisible by 16");

        _Resize(m_capacity);
    }

    FlatHashMap(size_t initCapacity, const _Allocator & alloc)
    : m_occupied(0), m_capacity(initCapacity), _Allocator(alloc)
    {
        assert(initCapacity % 16 == 0 && "Initial capacity must be divisible by 16");

        _Resize(m_capacity);
    }

    FlatHashMap(_Allocator && alloc)
    : m_occupied(0), m_capacity(16), _Allocator(alloc)
    {
        _Resize(m_capacity);
    }

    FlatHashMap(ContainerType && right) noexcept(std::is_nothrow_move_constructible<_Allocator>::value)
    :   _Allocator(std::move(right)), 
        m_occupied(std::move(right.m_occupied)), 
        m_capacity(std::move(right.m_capacity)), 
        m_slots(std::move(right.m_slots)),
        m_metadata(std::move(right.m_metadata))
    {
        right.m_occupied = 0;
        right.m_capacity = 0;
        right.m_slots = nullptr;
        right.m_metadata = nullptr;
    }

    ~FlatHashMap() {
        _DestroyAll();
        _Deallocate();
    }

    ContainerType& operator=(const ContainerType& right) noexcept {
        
        return *this;
    }

    ContainerType& operator=(ContainerType && right) noexcept {
        if (this == std::addressof(right)) return *this;

        _Allocator& _Al                 = *this;
        _Allocator& _Right_al           = right;
        constexpr bool _Propagate = std::allocator_traits<_Allocator>::propagate_on_container_move_assignment::value;

        if constexpr (_Propagate) {
            if (_Al != _Right_al) {
                auto&& _Alproxy       = static_cast<std::allocator_traits<_Allocator>::template rebind_alloc<std::pair<const K, V>>>(_Al);
                auto&& _Right_alproxy = static_cast<std::allocator_traits<_Allocator>::template rebind_alloc<std::pair<const K, V>>>(_Right_al);

                _Alproxy.operator=(std::move(_Right_alproxy));

                m_slots     = std::exchange(right.m_slots, nullptr);
                m_metadata  = std::exchange(right.m_metadata, nullptr);
                m_occupied  = std::exchange(right.m_occupied, 0);
                m_capacity  = std::exchange(right.m_capacity, 0);

                return *this;
            }
        } else {
            if (_Al != _Right_al) {
                auto&& _Alproxy       = static_cast<std::allocator_traits<_Allocator>::template rebind_alloc<std::pair<const K, V>>>(_Al);
                auto&& _Right_alproxy = static_cast<std::allocator_traits<_Allocator>::template rebind_alloc<std::pair<const K, V>>>(_Right_al);

                _DestroyAll();
                _Deallocate();

                m_capacity  = std::exchange(right.m_capacity, 0);
                
                _Allocate();

                for (auto it = right.begin(); it != right.end(); ++it) {
                    emplace(std::move(it->first), std::move(it->second));
                }

            }
        }

        m_slots     = std::exchange(right.m_slots, nullptr);
        m_metadata  = std::exchange(right.m_metadata, nullptr);
        m_occupied  = std::exchange(right.m_occupied, 0);
        m_capacity  = std::exchange(right.m_capacity, 0);
         
        _Allocator::operator=(right);
        
        return *this;
    }

    inline Slot* Insert(const K&  key, const V& value) {
        const uint64_t hash = _FlatHashMapHash<HashFunction, K>::operator()(key);
        const uint8_t h2 = hash & 0x7F;
        size_t idx = ((hash >> 7) & (m_capacity - 1)) & ~15;

        const simd128i vecEmpty = simdSet128i((char)0x80);
        const simd128i vecDeleted = simdSet128i((char)0xFE);
        const simd128i vecH2 = simdSet128i((char)h2);

        size_t firstTombstone = (size_t)-1;

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
                    return nullptr;
                }
                matchMask &= matchMask - 1;
            }

            if (firstTombstone == (size_t)-1) {
                const simd128i cmpDeleted = simdCmpEq8(metadata, vecDeleted);
                const uint16_t deletedMask = simdMovemask8(cmpDeleted);
                if(deletedMask) {
                    uint16_t firstDeletedBit = std::countr_zero(deletedMask);
                    firstTombstone = idx + firstDeletedBit;
                }
            }

            if (emptyMask) {
                if (firstTombstone != (size_t)-1) {
                    new (&m_slots[firstTombstone]) Slot{key, value};
                    m_metadata[firstTombstone] = h2;
    
                    m_occupied++;
                    return &m_slots[firstTombstone];
                } else {
                    const uint16_t bit = std::countr_zero(emptyMask);
                    const size_t slot_idx = (idx + bit) & (m_capacity - 1);
    
                    new (&m_slots[slot_idx]) Slot{key, value};
                    m_metadata[slot_idx] = h2;
    
                    m_occupied++;
                    return &m_slots[slot_idx];
                }
            }

            idx = (idx + 16) & (m_capacity - 1);
        }

        return nullptr;
    }

    template <class _VTy>
    inline Slot* Emplace(const K&  key, _VTy && value) {
        if(m_occupied >= m_capacity * 0.75) {
            _Rehash(m_capacity * 2);
        }

        return _Emplace(key, std::forward<_VTy>(value));
    }

    inline V* Find(const K& key) const {
        const uint64_t hash = _FlatHashMapHash<HashFunction, K>::operator()(key);
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
                    return &m_slots[slot_idx].second;
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

    inline void Erase(const K& key) {
        Slot* slot = _FindSlot(key);
        if (!slot) return;

        size_t idx = slot - m_slots;
        slot->~Slot();
        m_metadata[idx] = 0xFE;
        m_occupied--;
    }

    inline Iterator Erase(const Iterator& it) {
        if (!it) return it;

        size_t idx = it.m_index;
        m_slots[idx]->~Slot();
        m_metadata[idx] = 0xFE;
        m_occupied--;
        return Iterator(m_capacity, m_slots, m_metadata, ++idx);
    }

    inline void Clear() {
        _DestroyAll();

        simd128i empty = simdSet128i(0x80);
        for (size_t i = 0; i < m_capacity; i += 16) {
            _mm_store_si128((simd128i*)&m_metadata[i], empty);
        }
    }

    inline V* operator[](const K& key) const { return Find(key); }

    inline Iterator begin() {return Iterator(this, 0);}
    inline Iterator end() {return Iterator(this, m_capacity);}
    
    inline const Iterator begin() const {return Iterator(this, 0);}
    inline const Iterator end() const {return Iterator(this, m_capacity);}

    inline bool Empty() const {return m_occupied;}
    inline size_t Size() const {return m_occupied;}
protected:
    template <class _VTy>
    inline Slot* _Emplace(const K& key, _VTy && value) {
        const uint64_t hash = _FlatHashMapHash<HashFunction, K>::operator()(key);
        const uint8_t h2 = hash & 0x7F;
        size_t idx = ((hash >> 7) & (m_capacity - 1)) & ~15;

        const simd128i vecEmpty = simdSet128i((char)0x80);
        const simd128i vecDeleted = simdSet128i((char)0xFE);
        const simd128i vecH2 = simdSet128i((char)h2);

        size_t firstTombstone = (size_t)-1;

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

            if (firstTombstone == (size_t)-1) {
                const simd128i cmpDeleted = simdCmpEq8(metadata, vecDeleted);
                const uint16_t deletedMask = simdMovemask8(cmpDeleted);
                if(deletedMask) {
                    uint16_t firstDeletedBit = std::countr_zero(deletedMask);
                    firstTombstone = idx + firstDeletedBit;
                }
            }

            if (emptyMask) {
                if (firstTombstone != (size_t)-1) {
                    new (&m_slots[firstTombstone]) Slot{key, std::forward<_VTy>(value)};
                    m_metadata[firstTombstone] = h2;
    
                    m_occupied++;
                    return &m_slots[firstTombstone];
                } else {
                    const uint16_t bit = std::countr_zero(emptyMask);
                    const size_t slot_idx = (idx + bit) & (m_capacity - 1);
    
                    new (&m_slots[slot_idx]) Slot{key, std::forward<_VTy>(value)};
                    m_metadata[slot_idx] = h2;
    
                    m_occupied++;
                    return &m_slots[slot_idx];
                }
            }

            idx = (idx + 16) & (m_capacity - 1);
        }

        return nullptr;
    }

    inline Slot* _FindSlot(const K& key) const {
        const uint64_t hash = _FlatHashMapHash<HashFunction, K>::operator()(key);
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

    void _DestroyAll() {
        for(int i = 0; i < m_occupied; --i) {
            if (m_metadata[i] != 0x80 && m_metadata[i] != 0xFE) {
                m_slots[i].~Slot();
            }
        }

        m_occupied = 0;
    }

    void _Allocate() {
        MetaAllocator metaAlloc(*this);
        m_slots = std::allocator_traits<_Allocator>::allocate(*this, m_capacity);
        m_metadata = std::allocator_traits<MetaAllocator>::allocate(metaAlloc, m_capacity + 16);
        memset(m_metadata, 0x80, m_capacity + 16);
    }

    void _Rehash(size_t newCapacity) {
        Slot* oldSlots = m_slots;
        uint8_t* oldMetadata = m_metadata;
        size_t oldCapacity = m_capacity;

        m_occupied = 0;
        m_capacity = newCapacity;
        
        MetaAllocator metaAlloc(*this);
        m_slots = std::allocator_traits<_Allocator>::allocate(*this, m_capacity);
        m_metadata = std::allocator_traits<MetaAllocator>::allocate(metaAlloc, m_capacity + 16);
        memset(m_metadata, 0x80, m_capacity + 16);

        for (size_t i = 0; i < oldCapacity; ++i) {
            if (oldMetadata[i] != 0x80 && oldMetadata[i] != 0xFE) {
                _Emplace(std::move(oldSlots[i].first), std::move(oldSlots[i].second));
                oldSlots[i].~Slot();
            }
        }

        std::allocator_traits<_Allocator>::deallocate(*this, oldSlots, oldCapacity);
        std::allocator_traits<MetaAllocator>::deallocate(metaAlloc, oldMetadata, oldCapacity + 16);
    }

    void _Resize(size_t newCapacity) {
        assert(newCapacity % 16 == 0 && "Initial capacity must be divisible by 16");

        if(m_metadata && m_slots ) {
            for(int i = 0; i < m_occupied; --i) {
                if (m_metadata[i] != 0x80 && m_metadata[i] != 0xFE) {
                    m_slots[i].~Slot();
                }
            }
        } else {
            if(m_slots) {
                std::allocator_traits<_Allocator>::deallocate(*this, m_slots, m_capacity);
            }

            if(m_metadata) {
                MetaAllocator metaAlloc(*this);
                std::allocator_traits<MetaAllocator>::deallocate(metaAlloc, m_metadata, m_capacity + 16);
            }
        }

        MetaAllocator metaAlloc(*this);
        m_slots = std::allocator_traits<_Allocator>::allocate(*this, m_capacity);
        m_metadata = std::allocator_traits<MetaAllocator>::allocate(metaAlloc, m_capacity + 16);
        memset(m_metadata, 0x80, m_capacity + 16);
    }

    void _Deallocate() {
        if(m_slots) {
            std::allocator_traits<_Allocator>::deallocate(*this, m_slots, m_capacity);
        }

        if(m_metadata) {
            MetaAllocator metaAlloc(*this);
            std::allocator_traits<MetaAllocator>::deallocate(metaAlloc, m_metadata, m_capacity + 16);
        }
    }
};
#pragma once

#include "Asset.h"

#include <GarbageCollector/ReferenceCounterPtr.h>

template <AssetClass AssetT = Asset>
class AssetPtr : public ReferenceCounterPtr
{
protected:
    std::string_view m_path = "";
    AssetT* m_ptr = nullptr;
public:
    // Creation constructors
    AssetPtr() {}
    AssetPtr(AssetT* asset) : m_ptr(asset) {
        if(m_ptr) { 
            m_path = m_ptr->GetPath();
            ReferenceCounterPtr::AddRef(m_ptr);
        }
    }
    AssetPtr(std::string_view path, bool load = false) : m_path(path) {
        if(load){
            m_ptr = Load();
        }
    }
    AssetPtr(std::string_view path, AssetT* record) : m_path(path), m_ptr(record) { }

    // Copy constructor
    AssetPtr(const AssetPtr& other) : m_ptr(other.m_ptr) {
        if (m_ptr) ReferenceCounterPtr::AddRef(m_ptr);
    }

    // Copy assignment
    AssetPtr& operator=(const AssetPtr& other) {
        if (this != &other) {
            if (m_ptr) ReferenceCounterPtr::ReleaseRef(m_ptr);
            m_path = other.m_path;
            m_ptr = other.m_ptr;
            if (m_ptr) ReferenceCounterPtr::AddRef(m_ptr);
        }
        return *this;
    }

    // Move constructor
    AssetPtr(AssetPtr&& other) noexcept : m_path(other.m_path), m_ptr(other.m_ptr) {
        other.m_ptr = m_ptr;
        other.m_path = "";
    }
    
    // Move assignment
    AssetPtr& operator=(AssetPtr&& other) noexcept {
        if (this != &other) {
            if (m_ptr) ReferenceCounterPtr::ReleaseRef(m_ptr);
            m_path = other.m_path;
            m_ptr = other.m_ptr;
            other.m_path = "";
            other.m_ptr = nullptr;
        }
        return *this;
    }

    ~AssetPtr() {
        if(m_ptr) {
            ReferenceCounterPtr::ReleaseRef(m_ptr);
        }
    }

    AssetT* Load() {
        if(m_ptr) {
            return m_ptr;
        }

        // TODO Loading from AssetPtr
        return m_ptr;
    }

    AssetT* Get() {
        return m_ptr;
    }

    void Release() {
        if(m_ptr) {
            ReferenceCounterPtr::ReleaseRef(m_ptr);
            m_ptr = nullptr;
        }
    }

    void Reset() {
        Release();
        m_path = "";
    }

    // TODO: WeakAssetPtr
    /*WeakAssetPtr<AssetT> GetWeak() {
        return WeakAssetPtr<AssetT>(m_path, m_ptr);
    }*/

    bool Exists() const { return false; }
    bool IsBound() const { return m_ptr != nullptr; }
    bool IsValid() const {{ return m_ptr; }}
};
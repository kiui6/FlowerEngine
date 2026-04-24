#pragma once

#include "Asset.h"

#include <Platform/File.h>

#include <memory>
#include <unordered_map>

struct AssetFactory
{
    virtual std::unique_ptr<Asset> CreateFromFile(File* file) { return nullptr; }
};

class AssetFactoryLibrary
{
    std::unordered_map<uint64_t, std::unique_ptr<AssetFactory>> m_factories;
public:
    static AssetFactoryLibrary& Get() {
        static AssetFactoryLibrary library;
        return library;
    }

    template <typename FactoryType>
    static bool RegisterFactory(uint64_t id) {
        if(m_factories.find(id) != m_factories.end()) {
            return false;
        }
        
        m_factories.insert(id, std::move(std::make_unique<FactoryType>()));
        return true;
    }
};
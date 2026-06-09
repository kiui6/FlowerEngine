#pragma once

#include "Asset.h"

#include <Platform/Abstract/Filesystem/File.h>
#include <Data/DataView.h>

#include <memory>
#include <unordered_map>

struct AssetFactory
{
    virtual ID32 GetAssetType() const {return 0;}
    virtual std::unique_ptr<Asset> CreateFromFile(const DataView& view) { return nullptr; }
    virtual bool IsOfType(const DataView& view) { return false; }
};

template <AssetClass T>
struct TypedAssetFactory : public AssetFactory
{
    ID32 GetAssetType() const override {return T::StaticType();}
};

class AssetFactoryLibrary
{
    std::vector<std::unique_ptr<AssetFactory>> m_factories;
public:
    static AssetFactoryLibrary& Get() {
        static AssetFactoryLibrary library;
        return library;
    }

    bool RegisterFactory() {
        return true;
    }

    AssetFactory* FetchLibraryByType(ID32 assetType) {
        for(auto& factory : m_factories) {
            if(factory->GetAssetType() == assetType) {
                return factory.get();
            }
        }
        return nullptr;
    }

    AssetFactory* FetchLibraryByData(const DataView& view) {
        for(auto& factory : m_factories) {
            if(factory->IsOfType(view)) {
                return factory.get();
            }
        }
        return nullptr;
    }

    AssetFactory* FetchLibraryByTypeAndData(ID32 assetType, const DataView& view) {
        for(auto& factory : m_factories) {
            if(factory->GetAssetType() == assetType && factory->IsOfType(view)) {
                return factory.get();
            }
        }
        return nullptr;
    }
};
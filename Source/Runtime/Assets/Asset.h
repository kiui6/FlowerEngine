#pragma once

#include <cstdint>

#include <GarbageCollector/ReferenceCounter.h>

#include <Utility/ID.h>
#include <Mixin/Reflected.h>

#include <Data/DataView.h>

// TODO: Reflection
class Asset : public ReferenceCounter, public Typed
{
    virtual void RequestSelfDestruction() override;
protected:
    std::string m_path;
public:
    Asset() : Typed(Asset::StaticType()) {}
    Asset(std::string_view path) : Typed(Asset::StaticType()), m_path(path) {}
    virtual ~Asset(){}

    static ID32 StaticType() {return MakeID32("UNKN");}

    const std::string& GetPath() {return m_path;}

    virtual bool IsValid() const { return false; }
};

template<typename T>
concept AssetClass = std::is_base_of<Asset, T>::value;
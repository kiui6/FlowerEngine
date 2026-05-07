#pragma once

#include <cstdint>

#include <GarbageCollector/ReferenceCounter.h>

#include <Utility/ID.h>

class Asset : public ReferenceCounter
{
    virtual void RequestSelfDestruction() override;
protected:
    std::string m_path;
public:
    Asset() {}
    Asset(std::string_view path) : m_path(path) {}
    virtual ~Asset(){}

    static ID32 StaticType() {return MakeID32("UNKN");}

    const std::string& GetPath() {return m_path;}
};

template<typename T>
concept AssetClass = std::is_base_of<Asset, T>::value;
#pragma once

#include <cstdint>

#include <GarbageCollector/ReferenceCounter.h>

class Asset : public ReferenceCounter
{
    virtual void RequestSelfDestruction() override;
protected:
    std::string m_path;
public:
    virtual ~Asset(){}

    static uint64_t GetStaticType() {return 0;}

    const std::string& GetPath() {return m_path;}
};

template<typename T>
concept AssetClass = std::is_base_of<Asset, T>::value;
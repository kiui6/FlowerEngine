#pragma once

#include "RecordFactory.h"

#include <unordered_map>

class RecordFactoryLibrary
{
    std::unordered_map<uint32_t, std::unique_ptr<RecordFactory>> m_factories;
public:
    static RecordFactoryLibrary& Get() {
        static RecordFactoryLibrary lib;
        return lib; 
    }

    void RegisterFactory(uint32_t type, RecordFactory* factory) {
        m_factories.emplace(type, std::unique_ptr<RecordFactory>(factory));
    }

    RecordFactory* GetFactory(uint32_t type) {
        auto pair = m_factories.find(type);
        if(pair == m_factories.end()) {
            return nullptr;
        }
        return pair->second.get();
    }
};

template<typename T>
concept RecordFactoryClass = std::is_base_of<RecordFactory, T>::value;

template <RecordFactoryClass RecordFactoryType>
bool RegisterRecordFactory()
{
    RecordFactoryType* factory = new RecordFactoryType;
    RecordFactoryLibrary::Get().RegisterFactory(factory->GetRecordType(), factory);
    return true;
}

template <RecordClass RecordType>
bool RegisterTemplatedRecordFactory()
{
    RecordFactory* factory = new TemplatedRecordFactory<RecordType>();
    RecordFactoryLibrary::Get().RegisterFactory(factory->GetRecordType(), factory);
    return true;
}
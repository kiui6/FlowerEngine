#pragma once

#include "ActorFactory.h"

#include <unordered_map>

class ActorFactoryLibrary
{
    std::unordered_map<uint32_t, std::unique_ptr<ActorFactory>> m_factories;
public:
    static ActorFactoryLibrary& Get() {
        static ActorFactoryLibrary lib;
        return lib; 
    }

    void RegisterFactory(uint32_t type, ActorFactory* factory) {
        m_factories.emplace(type, std::unique_ptr<ActorFactory>(factory));
    }

    ActorFactory* GetFactory(uint32_t type) {
        auto pair = m_factories.find(type);
        if(pair == m_factories.end()) {
            return nullptr;
        }
        return pair->second.get();
    }
};

template<typename T>
concept ActorFactoryClass = std::is_base_of<ActorFactory, T>::value;

template <ActorFactoryClass ActorFactoryType>
bool RegisterActorFactory()
{
    ActorFactoryType* factory = new ActorFactoryType;
    ActorFactoryLibrary::Get().RegisterFactory(factory->GetRecordType(), factory);
    return true;
}
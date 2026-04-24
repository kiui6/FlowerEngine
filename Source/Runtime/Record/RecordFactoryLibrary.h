#pragma once

#include "RecordFactory.h"

#include <unordered_map>

class RecordFactoryLibrary
{
    std::unordered_map<uint32_t, std::unique_ptr<RecordFactory>> m_factories;
public:
    RecordFactoryLibrary& Get() {
        static RecordFactoryLibrary lib;
        return lib; 
    }
};

template<typename T>
concept RecordFactoryClass = std::is_base_of<RecordFactory, T>::value;

template <RecordFactoryClass RecordFactoryType>
bool RegisterRecordFactory()
{
    return true;
}
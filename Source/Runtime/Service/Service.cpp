#include "Service.h"

#include <cassert>

#include <Log/Log.h>

ServiceProvider::~ServiceProvider()
{
    for(auto it = m_services.begin(); it != m_services.end(); ++it)
    {
        delete it->second;
    }
}

void ServiceProvider::Initialize()
{
    std::deque<ServiceArchetype> sortedArchetypes;

    // TODO: Sort dependencies
    for(auto archetype : m_serviceArchetypes) {
        sortedArchetypes.push_front(archetype);
    }

    while(!sortedArchetypes.empty()) {
        auto archetype = sortedArchetypes.front();
        IService* service = archetype.m_instantiatorFunc();
        service->Initialize();
        AddService(service);
        sortedArchetypes.pop_front();
    }
}

void ServiceProvider::Deinitialize()
{
}

bool ServiceProvider::RegisterServiceArchetype(ServiceArchetype &archetype)
{
    m_serviceArchetypes.push_back(archetype);
    return true;
}

IService *ServiceProvider::GetServiceByName(std::string_view name)
{
    auto it = m_services.find(name);
    if(it == m_services.end())
    {
        return nullptr;
    }
    
    return it->second;
}

void ServiceProvider::AddService(IService *service)
{
    assert(service);

    m_services.insert_or_assign(service->GetName(), service);
    m_servicesOrder.push_front(service->GetName());
}

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

    while(m_serviceArchetypes.size() != 0) {
        for(int i = 0; i < m_serviceArchetypes.size(); i++) {
            ServiceArchetype archetypeBeingSolved = m_serviceArchetypes[i];

            bool bDependenciesUnmet = false;

            // Find unsorted dependencies
            for(std::string_view& dependency : archetypeBeingSolved.dependencyNames) {
                if(std::ranges::find_if(m_serviceArchetypes.begin(), 
                                m_serviceArchetypes.end(), 
                                [dependency](const ServiceArchetype& element){return dependency == element.serviceName;}) 
                    != m_serviceArchetypes.end())
                {
                    bDependenciesUnmet = true;
                    break;
                }
            }
            
            if(bDependenciesUnmet) {
                continue;
            }

            sortedArchetypes.push_front(m_serviceArchetypes[i]);
            m_serviceArchetypes.erase(m_serviceArchetypes.begin() + i);
            --i;
        }
    }

    while(!sortedArchetypes.empty()) {
        auto archetype = sortedArchetypes.back();
        IService* service = archetype.instantiatorFunc();
        service->Initialize();
        AddService(service);
        sortedArchetypes.pop_back();
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

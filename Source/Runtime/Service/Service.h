#pragma once

#include <string_view>
#include <unordered_map>
#include <concepts>
#include <functional>
#include <queue>
#include <memory>

class IService;

template<typename T>
concept ServiceClass = std::is_base_of<IService, T>::value;

template <ServiceClass ServiceType>
struct ServiceInstantiator;

/*
 * Services are global objects that can be acquired from any thread and method
 * It means that most services must implement mutex to avoid race condition
 */
class IService
{
    friend class ServiceProvider;
    template <ServiceClass ServiceType>
    friend struct ServiceInstantiator;
    std::string_view m_name;
public:
    IService() = default;
    virtual ~IService() = default;

    static std::string_view GetStaticName() {return "Service";}
    std::string_view GetName() const {return m_name;}

    virtual void Initialize() {}
    virtual void Deinitialize() {}
};

template <ServiceClass ServiceType>
struct ServiceInstantiator {
    static std::function<std::unique_ptr<IService>()> Get() {
        return [](){
            std::unique_ptr<IService> service = std::make_unique<ServiceType>();
            service->m_name = ServiceType::GetStaticName();
            return service;
        };
    }
};

struct ServiceArchetype {
    std::function<std::unique_ptr<IService>()> instantiatorFunc;
    std::string_view serviceName;
    std::vector<std::string_view> dependencyNames;
};

class ServiceProvider
{   
    std::vector<ServiceArchetype> m_serviceArchetypes;

    std::unordered_map<std::string_view, std::unique_ptr<IService>> m_services;
    std::deque<std::string_view> m_servicesOrder;
public:
    ~ServiceProvider();

    static ServiceProvider& Get()
    {
        static ServiceProvider provider;
        return provider;
    }

    void Initialize();
    void Deinitialize();

    bool RegisterServiceArchetype(ServiceArchetype &archetype);
    IService* GetServiceByName(std::string_view name);
private:
    void AddService(std::unique_ptr<IService> && service);
};

template <ServiceClass ServiceType>
inline bool RegisterService(std::vector<std::string_view> dependencies = {})
{
    ServiceProvider &provider = ServiceProvider::Get();
    
    ServiceArchetype archetype = {
        .instantiatorFunc = ServiceInstantiator<ServiceType>::Get(),
        .serviceName = ServiceType::GetStaticName(),
        .dependencyNames = dependencies
    };
    
    bool bSuccess = provider.RegisterServiceArchetype(archetype);

    return bSuccess;
}

template <ServiceClass ServiceType>
inline bool RegisterEditorService(std::vector<std::string_view> dependencies = {})
{
    #ifdef EDITOR
        return RegisterService<ServiceType>(dependencies);  
    #else
        return false;
    #endif
}

template <ServiceClass ServiceType>
inline bool RegisterRuntimeService(std::vector<std::string_view> dependencies = {})
{
    #ifndef EDITOR
        return RegisterService<ServiceType>(dependencies);
    #else
        return false;
    #endif
}

template <ServiceClass ServiceType>
inline ServiceType* GetService()
{
    ServiceProvider &provider = ServiceProvider::Get();
    return static_cast<ServiceType*>(provider.GetServiceByName(ServiceType::GetStaticName()));
}

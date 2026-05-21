#pragma once

#include <Service/Service.h>

#include <unordered_map>
#include <Utility/Hash.h>

class ConfigNamespace {
    template <typename _ValT>
    using Container = std::unordered_map<std::string, _ValT, StringHash, std::equal_to<>>;

    Container<std::string> m_strings;
    Container<int> m_ints;
    Container<float> m_floats;
    Container<bool> m_bools;
public:

    int GetInt(std::string_view name, int defaultValue = 0);
    void SetInt(std::string_view name, int value);

    std::string_view GetString(std::string_view name, std::string_view defaultValue = "");
    void SetString(std::string_view name, std::string_view value);
};

class Config : public IService
{
    static bool bIsInitialized;

    std::unordered_map<std::string, ConfigNamespace, StringHash, std::equal_to<>> m_namespaces;
public:
    static std::string_view GetStaticName() {return "Config";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    ConfigNamespace& GetNamespace(std::string_view name);
};
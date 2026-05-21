#include "Config.h"

#include <Platform/Platform.h>

bool Config::bIsInitialized = RegisterService<Config>({Platform::GetStaticName()});

void Config::Initialize()
{
}

void Config::Deinitialize()
{
}

ConfigNamespace& Config::GetNamespace(std::string_view name)
{
    auto it = m_namespaces.find(name);
    if(it != m_namespaces.end()) {
        return it->second;
    }

    return m_namespaces.emplace(name, ConfigNamespace{}).first->second;
}

int ConfigNamespace::GetInt(std::string_view name, int defaultValue)
{
    auto it = m_ints.find(name);
    if(it != m_ints.end()) {
        return it->second;
    }

    return defaultValue;
}

std::string_view ConfigNamespace::GetString(std::string_view name, std::string_view defaultValue)
{
    auto it = m_strings.find(name);
    if(it != m_strings.end()) {
        return it->second;
    }

    return defaultValue;
}

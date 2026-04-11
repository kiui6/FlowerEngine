#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <array>
#include <type_traits>

#include "ComponentTemplate.h"
#include "../Component.h"

class EntityComponentTemplateLibrary
{
    uint16_t m_currentID = 0;
    std::map<uint16_t, std::pair<CompiledEntityComponentTemplate(*), EntityComponent(*)>> m_templates;
    std::unordered_map<std::string_view, uint16_t> m_templateIDs;
public:
    static EntityComponentTemplateLibrary& Get();

    template <typename ComponentTemplateType, typename ComponentType> 
    requires requires (){
        std::is_base_of_v<CompiledEntityComponentTemplate, ComponentTemplateType>;
        std::is_base_of_v<EntityComponent, ComponentType>;
    }
    static bool RegisterComponentTemplate(std::string_view typeName);
};

template <typename ComponentTemplateType, typename ComponentType> 
requires requires (){
    std::is_base_of_v<CompiledEntityComponentTemplate, ComponentTemplateType>;
    std::is_base_of_v<EntityComponent, ComponentType>;
}
inline bool EntityComponentTemplateLibrary::RegisterComponentTemplate(std::string_view typeName)
{
    return false;
}

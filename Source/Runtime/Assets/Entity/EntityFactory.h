#pragma once

#include "EntityTemplate/EntityTemplateRegistry.h"

/*
 * EntityFactory produces new entity actors from CompiledEntityTemplates at runtime
 * 
 * It's unique for every Procedural World and contains all entity templates in a registry
 */
class EntityFactory
{
    EntityTemplateRegistry m_templateRegistry;
};
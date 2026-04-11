#pragma once

#include <string>

#include "EntityTemplate.h"

class EntityTemplateCompiler
{
public:
    static CompiledEntityTemplate Compile(std::string json);
};
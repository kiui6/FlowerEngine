#pragma once

#include <string>

#include "BehaviorTemplate.h"

class BehaviorTemplateCompiler
{
public:
    static CompiledBehaviorTemplate Compile(std::string json);
};
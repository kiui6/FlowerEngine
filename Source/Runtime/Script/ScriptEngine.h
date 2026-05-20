#pragma once

#include <Service/Service.h>

#include "ScriptVM.h"

class ScriptEngine : public IService
{
    static bool bIsInitialized;

    ScriptVM m_vm;
public:
    ScriptEngine();

    static std::string_view GetStaticName() {return "ScriptEngine";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;
};

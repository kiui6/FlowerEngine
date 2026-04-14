#pragma once

#include <Service/Service.h>
#include "Text.h"

class LocaleManager : public IService
{
    static bool bIsInitialized;
public:
    static std::string_view GetStaticName() {return "LocaleManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    std::optional<std::string_view> FetchLocaleString(std::string_view id);
};
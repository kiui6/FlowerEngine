#pragma once

#include <Service/Service.h>
#include "Text.h"

#include <unordered_map>
#include <string_view>
#include <optional>

class LocaleManager : public IService
{
    static bool bIsInitialized;

    std::unordered_map<std::string_view, std::string_view> locale;
public:
    static std::string_view GetStaticName() {return "LocaleManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    void LoadLocale(std::string localeName);
    void Reset();

    std::optional<std::string_view> FetchLocaleString(std::string_view id);
};
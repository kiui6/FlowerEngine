#pragma once

#include "../PlatformImpl.h"
#include "WindowsMappedFile.h"

class PlatformImpl : public IPlatformImpl
{
public:
    std::string_view GetPlatformName() const override {return "Windows";}
    std::unique_ptr<IMappedFile> CreateMappedFile() override {return std::make_unique<WindowsMappedFile>();}
};
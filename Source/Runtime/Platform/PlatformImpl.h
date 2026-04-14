#pragma once

#include <string>
#include <memory>

#include "MappedFile.h"

struct IPlatformImpl
{
    virtual std::string_view GetPlatformName() const {return "Unknown";}

    virtual std::unique_ptr<IMappedFile> CreateMappedFile() {return nullptr;}
};
#pragma once

#include "PlatformDefines.h"

#include <string>

namespace Platform
{
    void DebugPrint(const char* string);

    namespace FileSystem {
        std::string GetLocalDirectory();
    }
};
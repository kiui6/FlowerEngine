#include "../Platform.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma comment(lib, "Kernel32.lib")

namespace Platform
{
    void DebugPrint(const char* string)
    {
        OutputDebugStringA(string);
    }

    namespace FileSystem {
        std::string GetLocalDirectory()
        {
            return "";
        }
    }
};
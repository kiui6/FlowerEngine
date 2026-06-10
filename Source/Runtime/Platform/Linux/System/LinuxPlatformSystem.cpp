#include "LinuxPlatformSystem.h"

#include <cstdint>

void LinuxPlatformSystem::DebugPrint(const char *string)
{
    printf("%s\n", string);
}
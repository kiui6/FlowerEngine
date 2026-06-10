#include "LinuxPlatform.h"

#include "../Platform.h"

#include <cstdint>

void Platform::DebugPrint(const char *string)
{
    printf("%s\n", string);
}
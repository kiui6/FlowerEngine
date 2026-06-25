#include "MacPlatform.h"

#include "../Platform.h"

#include <cstdint>
#include <cstdio>

void Platform::DebugPrint(const char *string)
{
    printf("%s\n", string);
}
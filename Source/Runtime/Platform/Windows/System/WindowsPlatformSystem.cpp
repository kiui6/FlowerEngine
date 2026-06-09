#include "WindowsPlatformSystem.h"

#include "../WinAPI.h"

void WindowsPlatformSystem::DebugPrint(const char *string)
{
    OutputDebugStringA(string);
}
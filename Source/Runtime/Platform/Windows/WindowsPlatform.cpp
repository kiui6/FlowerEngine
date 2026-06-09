#include "WindowsPlatform.h"

#include "../Platform.h"

#include "WinAPI.h"

#pragma comment(lib, "Kernel32.lib")

void Platform::DebugPrint(const char *string)
{
    OutputDebugStringA(string);
}
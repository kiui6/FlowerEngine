#include "WindowsPlatform.h"

#include "../Platform.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma comment(lib, "Kernel32.lib")

void Platform::DebugPrint(const char* string)
{
    OutputDebugStringA(string);
}
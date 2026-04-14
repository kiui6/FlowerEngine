#pragma once

#if defined(_WIN32) || defined(WIN32) || defined(__MINGW64__)
#   define PLATFORM_WINDOWS
#elif __linux__
#   define PLATFORM_LINUX
#elif __unix__
#   define PLATFORM_UNIX
#elif __APPLE__
#   define PLATFORM_MAC
#endif
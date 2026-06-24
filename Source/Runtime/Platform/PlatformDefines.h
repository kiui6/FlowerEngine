#pragma once

enum class PlatformType {
    Windows,
    Linux,
    Mac
};

#ifdef PLATFORM_WINDOWS
    constexpr PlatformType PLATFORM_TYPE = PlatformType::Windows;
#elifdef PLATFORM_LINUX
    constexpr PlatformType PLATFORM_TYPE = PlatformType::Linux;
#elifdef PLATFORM_MAC
    constexpr PlatformType PLATFORM_TYPE = PlatformType::Mac;
#endif

#ifdef EDITOR
    constexpr bool IS_EDITOR = true;
#else
    constexpr bool IS_EDITOR = false;
#endif

constexpr bool IS_VERBOSE = true;
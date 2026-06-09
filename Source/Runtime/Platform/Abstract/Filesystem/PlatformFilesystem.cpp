#include "PlatformFilesystem.h"

#include <SDL3/SDL.h>

std::string PlatformFilesystem::GetBaseDirectory()
{
    return SDL_GetBasePath();
}

std::string PlatformFilesystem::GetPreferencesDirectory(const std::string& org, const std::string& app)
{
    // TODO: Get pref path for the game
    return SDL_GetPrefPath(org.c_str(), app.c_str());
}

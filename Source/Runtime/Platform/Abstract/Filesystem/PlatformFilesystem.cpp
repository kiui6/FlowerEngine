#include "PlatformFilesystem.h"

#include <SDL3/SDL.h>

std::string PlatformFilesystem::GetBaseDirectory()
{
    return SDL_GetBasePath();
}

std::string PlatformFilesystem::GetPreferencesDirectory(std::string_view org, std::string_view app)
{
    return SDL_GetPrefPath(org.data(), app.data());
}

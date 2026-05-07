#include "AssetLoadCapable.h"

#include <Assets/Asset.h>
#include <Assets/AssetLibrary.h>

Asset *AssetLoadCapable::LoadAsset(std::string_view path)
{
    return GetService<AssetLibrary>()->Load(path);
}
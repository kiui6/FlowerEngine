#include "Asset.h"

#include "AssetLibrary.h"

#include <Log/Log.h>
#include <cassert>

void Asset::RequestSelfDestruction()
{
    if(AssetLibrary* library = GetService<AssetLibrary>()) {
        if(m_path.empty()) {
            library->DestroyAsset(this);
        } else {
            library->UnloadAsset(m_path);
        }
    } else {
        LOG(Assert, LogAsset, "Attempted to destroy Asset, but AssetLibrary service doesn't exist");
    }
}
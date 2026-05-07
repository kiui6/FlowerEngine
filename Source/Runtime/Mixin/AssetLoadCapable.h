#pragma once

#include <string_view>

class Asset;

class AssetLoadCapable {
protected:
    Asset* LoadAsset(std::string_view path);
};
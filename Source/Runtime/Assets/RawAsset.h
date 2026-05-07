#pragma once

#include "Asset.h"
#include <Data/DataView.h>

#include <vector>

class RawAsset : public Asset
{
    std::vector<std::byte> m_data;
public:
    RawAsset(std::string_view path) : Asset(path) {}
    RawAsset(std::string_view path, DataView view);

    std::vector<std::byte>& GetData() { return m_data; }
};
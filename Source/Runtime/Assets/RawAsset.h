#pragma once

#include "Asset.h"
#include <Data/DataView.h>

#include <vector>

class RawAsset : public Asset
{
    std::vector<std::byte> m_data;
public:
    RawAsset(std::string_view path) {SetType(RawAsset::StaticType());}
    RawAsset(std::string_view path, DataView view);

    std::vector<std::byte>& GetData() { return m_data; }

    static ID32 StaticType() {return MakeID32("RAW_");}
};
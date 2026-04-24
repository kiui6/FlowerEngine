#pragma once

#include "Asset.h"

#include <vector>

class RawAsset : public Asset
{
    std::vector<unsigned char> m_data;
public:
    std::vector<unsigned char>& GetData();
};
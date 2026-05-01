#pragma once

#include <Assets/Asset.h>

#include <vector>

class Texture2DAsset : public Asset
{
    std::vector<std::byte> m_data;
public:
    std::vector<std::byte>& GetTextureData() {return m_data;}
};
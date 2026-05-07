#pragma once

#include <Assets/Asset.h>

#include <vector>

class Texture2DAsset : public Asset
{
    std::vector<std::byte> m_data;
    uint16_t m_width, m_height;
public:
    static ID32 StaticType() {return MakeID32("TEX2");}

    std::vector<std::byte>& GetTextureData() {return m_data;}
    uint16_t GetWidth() {return m_width;}
    uint16_t GetHeight() {return m_height;}
};
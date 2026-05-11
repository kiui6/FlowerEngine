#pragma once

#include <Assets/Asset.h>

#include <vector>
#include <memory>

#define STBI_NO_TGA
#define STBI_NO_HDR
#define STBI_NO_PNM
#define STBI_NO_JPEG
#define STBI_NO_PIC
#define STBI_NO_PSD
#define STBI_NO_BMP

#include <stb/stb_image.h>

class Texture2DAsset : public Asset
{
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> m_data;
    size_t m_dataSize;
    uint16_t m_width, m_height;
    uint8_t m_channels;
public:
    Texture2DAsset(std::string_view path) : Asset(path), m_data(std::unique_ptr<unsigned char, decltype(&stbi_image_free)>(nullptr, &stbi_image_free)) {SetType(Texture2DAsset::StaticType());}
    // TODO
    Texture2DAsset(std::string_view path, DataView view);

    static ID32 StaticType() {return MakeID32("TEX2");}

    virtual bool IsValid() const override { return m_data.get(); }

    unsigned char* GetTextureData() {return m_data.get();}
    size_t GetTextureDataSize() const { return m_dataSize; }
    uint16_t GetWidth() {return m_width;}
    uint16_t GetHeight() {return m_height;}
};
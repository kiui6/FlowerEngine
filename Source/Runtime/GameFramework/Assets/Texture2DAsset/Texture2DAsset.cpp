#include "Texture2DAsset.h"

#include <Log/Log.h>

Texture2DAsset::Texture2DAsset(std::string_view path, DataView view)
    : Asset(path), m_data(std::unique_ptr<unsigned char, decltype(&stbi_image_free)>(nullptr, &stbi_image_free))
{
    SetType(Texture2DAsset::StaticType());

    if(!view) {
        LOG(Error, LogTexture2DAsset, "Failed to read Texture 2D asset: invalid DataView");
        return;
    }

    const stbi_uc* inputBuffer = reinterpret_cast<const stbi_uc*>(view.data());

    int w, h, channels;

    m_data = std::unique_ptr<unsigned char, decltype(&stbi_image_free)>(
        stbi_load_from_memory(inputBuffer, view.size(), &w, &h, &channels, /*TODO: Maybe make another ways of texture conversion*/4),
        &stbi_image_free);
    if(!m_data) {
        LOG(Error, LogTexture2DAsset, "Failed to read Texture 2D asset: failed to parse file");
        return;
    }

    m_width = static_cast<uint16_t>(w);
    m_height = static_cast<uint16_t>(h);
    // TODO: Need better conversion handling instead in the future
    m_channels = 4;

    m_dataSize = static_cast<size_t>(w) * h * m_channels;
}
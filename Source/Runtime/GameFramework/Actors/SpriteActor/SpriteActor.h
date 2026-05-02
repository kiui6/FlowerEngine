#pragma once

#include <World/Actor/Actor.h>
#include <GameFramework/Records/AtlasRecord/AtlasRecord.h>
#include <GameFramework/Records/TextureRecord/TextureRecord.h>
#include <Assets/AssetPtr.h>
#include <GameFramework/Assets/Texture2DAsset/Texture2DAsset.h>

class SpriteActor : public Actor
{
    RecordPtr<AtlasRecord> m_atlas;

    RecordPtr<TextureRecord> m_albedo;
    RecordPtr<TextureRecord> m_relief;
    AssetPtr<Texture2DAsset> m_albedoData;
    AssetPtr<Texture2DAsset> m_reliefData;
    bool m_autogenRelief = false;
    RecordID m_autogenReliefID = INVALID_RECORD;

    uint16_t m_atlasIndex;
public:
    virtual void Initialize() override;

    virtual void RecordRenderView(RenderView& renderView) override;
};
#pragma once

#include <World/Actor/Actor.h>
#include <World/Actor/ActorFactory.h>
#include <GameFramework/Records/AtlasRecord/AtlasRecord.h>
#include <GameFramework/Records/TextureRecord/TextureRecord.h>
#include <Assets/AssetPtr.h>
#include <GameFramework/Assets/Texture2DAsset/Texture2DAsset.h>
#include <Graphics/RenderEngine/RenderView/RenderResource.h>

class SpriteActor : public Actor
{
    RecordPtr<AtlasRecord> m_atlas;

    RecordPtr<TextureRecord> m_albedo;
    RecordPtr<TextureRecord> m_relief;
    AssetPtr<Texture2DAsset> m_albedoData;
    AssetPtr<Texture2DAsset> m_reliefData;
    bool m_autogenRelief = false;
    RecordID m_autogenReliefID = INVALID_RECORD;

    /* GPU Resource Handles */
    Texture2DResource m_albedoRenderResource{};
    Texture2DResource m_reliefRenderResource{};

    uint16_t m_atlasIndex;
public:
    SpriteActor(const RecordPtr<ReferenceRecord>& reference);

    virtual void OnInput(const InputView& input) override {
        if(input.GetAction("moveU")) {
            auto loc = GetLocation();
            loc.y += 1;
            SetLocation(loc);
        }
        if(input.GetAction("moveL")) {
            auto loc = GetLocation();
            loc.x -= 1;
            SetLocation(loc);
        }
        if(input.GetAction("moveD")) {
            auto loc = GetLocation();
            loc.y -= 1;
            SetLocation(loc);
        }
        if(input.GetAction("moveR")) {
            auto loc = GetLocation();
            loc.x += 1;
            SetLocation(loc);
        }
    }

    virtual void RecordRenderView(RenderView& renderView) override;
};

struct SpriteActorFactory : public ActorFactory
{
    static bool s_Initialized; 
    virtual constexpr ID32 GetRecordType() override {return AtlasRecord::StaticType();}
protected:
    virtual std::unique_ptr<Actor> CreateActorImpl(const RecordPtr<ReferenceRecord> & reference) override {return std::make_unique<SpriteActor>(reference);}
};
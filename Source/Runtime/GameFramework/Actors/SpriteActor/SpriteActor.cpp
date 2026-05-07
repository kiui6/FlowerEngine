#include "SpriteActor.h"

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>
#include <Graphics/RenderElements/ReliefSpriteRenderElement.h>
#include <Graphics/OnDemandRenderService/OnDemandRenderService.h>
#include <Graphics/OnDemandTasks/AutogenReliefRenderTask.h>

#include <Record/RecordLibrary.h>

void SpriteActor::Initialize()
{
    if(GetReference()) {
        m_atlas = RecordPtr<AtlasRecord>(GetReference()->Base, true);

        m_albedo = RecordPtr<TextureRecord>(m_atlas->AlbedoTexture, true);
        m_autogenRelief = m_atlas->AlbedoTexture;
        m_relief = RecordPtr<TextureRecord>(m_atlas->ReliefTexture, true);

        if(m_albedo.IsBound())
            m_albedoData = AssetPtr<Texture2DAsset>(m_albedo->TexturePath, true);

        if(m_relief.IsBound())
            m_reliefData = AssetPtr<Texture2DAsset>(m_relief->TexturePath, true);
    }
}

void SpriteActor::RecordRenderView(RenderView &renderView)
{
    std::unique_ptr<RenderObject> ro = renderView.AllocateRenderObject();

    // Opaque Sprite Element
    if(m_albedo.IsBound() && m_albedoData.IsBound()) {
        OpaqueSpriteRenderElement* opaque = ro->CreateRenderElement<OpaqueSpriteRenderElement>();

        opaque->position = m_transform.Location;
        opaque->depth = m_transform.Depth;
        opaque->rotation = m_transform.Rotation;
        opaque->scale = m_transform.Scale;

        Texture2DResource albedoTex;
        albedoTex.components = Texture2DComponents::R8G8B8A8_UNORM;
        albedoTex.id = m_albedo.Get()->GetID();
        albedoTex.data = m_albedoData.Get()->GetTextureData();

        opaque->texture = albedoTex;
    }

    // Relief Sprite Element
    if(m_autogenRelief && m_albedo.IsBound() && m_albedoData.IsBound()) {
        // OnDemand Generate Relief Map Task
        if(m_autogenReliefID == INVALID_RECORD) {
            m_autogenReliefID = GetService<RecordLibrary>()->ReserveLocalRecordID();
            // Execute task
            GetService<OnDemandRenderService>()->Submit<AutogenReliefRenderTask>(m_albedoData->GetWidth(), m_albedoData->GetHeight(), m_albedoData->GetTextureData());
        } else {
            ReliefSpriteRenderElement* autogenRelief = ro->CreateRenderElement<ReliefSpriteRenderElement>();

            autogenRelief->position = m_transform.Location;
            autogenRelief->depth = m_transform.Depth;
            autogenRelief->rotation = m_transform.Rotation;
            autogenRelief->scale = m_transform.Scale;

            Texture2DResource reliefTex;
            reliefTex.components = Texture2DComponents::R16G16_UINT;
            reliefTex.id = m_autogenReliefID;
            reliefTex.data = {};

            autogenRelief->texture = reliefTex;
        }
    } else if(m_relief.IsBound() && m_reliefData.IsBound()) {

        ReliefSpriteRenderElement* relief = ro->CreateRenderElement<ReliefSpriteRenderElement>();

        relief->position = m_transform.Location;
        relief->depth = m_transform.Depth;
        relief->rotation = m_transform.Rotation;
        relief->scale = m_transform.Scale;

        Texture2DResource reliefTex;
        reliefTex.components = Texture2DComponents::R16G16_UINT;
        reliefTex.id = m_relief.Get()->GetID();
        reliefTex.data = m_reliefData.Get()->GetTextureData();

        relief->texture = reliefTex;   
    }

    renderView.AddDynamicRenderObject(m_ref.GetID(), std::move(ro));
}
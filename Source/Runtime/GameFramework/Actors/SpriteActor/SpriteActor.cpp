#include "SpriteActor.h"

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>
#include <Graphics/RenderElements/ReliefSpriteRenderElement.h>
#include <Graphics/RenderJobs/AutogenReliefRenderTask.h>

#include <Record/RecordLibrary.h>
#include <Assets/AssetLibrary.h>

void SpriteActor::Initialize()
{
    if(GetReference()) {
        RecordLibrary* reclib = GetService<RecordLibrary>();
        AssetLibrary* assetlib = GetService<AssetLibrary>();

        m_atlas = reclib->LoadRecord<AtlasRecord>(GetReference()->Base);

        m_albedo = reclib->LoadRecord<TextureRecord>(m_atlas->AlbedoTexture);
        m_autogenRelief = m_atlas->AutoReliefGeneration;
        m_relief = reclib->LoadRecord<TextureRecord>(m_atlas->ReliefTexture);

        if(m_albedo.IsBound()) {
            std::string path = m_albedo->TexturePath;
            m_albedoData = assetlib->LoadAsset<Texture2DAsset>(path);
        }

        if(m_relief.IsBound()) {
            std::string path = m_relief->TexturePath;
            m_reliefData = assetlib->LoadAsset<Texture2DAsset>(path);
        }
    }
}

void SpriteActor::RecordRenderView(RenderView &renderView)
{
    RenderObject* rendObj =  nullptr;
    if(GetReference()->IsDynamic) {
        rendObj = renderView.GetDynamicRenderObject(m_ref.GetID());
    } else {
        if(nullptr != renderView.GetStaticRenderObject(m_ref.GetID())) {
            return;
        }
        rendObj = renderView.AddStaticRenderObject(m_ref.GetID());
    }

    // Opaque Sprite Element
    if(m_albedo.IsBound() && m_albedoData.IsBound()) {
        OpaqueSpriteRenderElement* opaque = rendObj->CreateRenderElement<OpaqueSpriteRenderElement>();

        opaque->position = m_transform.Location;
        opaque->depth = m_transform.Depth;
        opaque->rotation = m_transform.Rotation;
        opaque->size = m_transform.Scale * Float2{m_albedoData->GetWidth(), m_albedoData->GetHeight()};

        if(m_albedoRenderResource.GetCompiledResource().expired()) {
            m_albedoRenderResource.components = Texture2DComponents::R8G8B8A8_UNORM;
            m_albedoRenderResource.id = m_albedo.Get()->GetID();
            m_albedoRenderResource.width = m_albedoData.Get()->GetWidth();
            m_albedoRenderResource.height = m_albedoData.Get()->GetHeight();
            m_albedoRenderResource.data = m_albedoData.Get()->GetTextureData();
            m_albedoRenderResource.dataSize = m_albedoData.Get()->GetTextureDataSize();
            m_albedoRenderResource.isDirty = true;
        }

        opaque->texture = &m_albedoRenderResource;
    }

    // Relief Sprite Element
    if(m_autogenRelief && m_albedo.IsBound() && m_albedoData.IsBound()) {
        ReliefSpriteRenderElement* autogenRelief = rendObj->CreateRenderElement<ReliefSpriteRenderElement>();

        // OnDemand Generate Relief Map Task
        if(m_reliefRenderResource.GetCompiledResource().expired()) {
            if(m_autogenReliefID == INVALID_RECORD) {
                m_autogenReliefID = GetService<RecordLibrary>()->ReserveLocalRecordID();
            }
            
            m_reliefRenderResource.components = Texture2DComponents::R16G16_UINT;
            m_reliefRenderResource.id = m_autogenReliefID;
            m_reliefRenderResource.width = m_albedoData.Get()->GetWidth();
            m_reliefRenderResource.height = m_albedoData.Get()->GetHeight();
            m_reliefRenderResource.data = m_albedoData->GetTextureData();
            m_reliefRenderResource.dataSize = m_albedoData->GetTextureDataSize();
            m_reliefRenderResource.isDirty = true;

            AutogenReliefRenderJob* autogenReliefJob = new AutogenReliefRenderJob(m_reliefRenderResource);  
            renderView.SubmitJob(autogenReliefJob);
        }

        autogenRelief->position = m_transform.Location;
        autogenRelief->depth = m_transform.Depth;
        autogenRelief->rotation = m_transform.Rotation;
        autogenRelief->scale = m_transform.Scale;

        autogenRelief->texture = &m_reliefRenderResource;
    } else if(m_relief.IsBound() && m_reliefData.IsBound()) {

        ReliefSpriteRenderElement* relief = rendObj->CreateRenderElement<ReliefSpriteRenderElement>();

        relief->position = m_transform.Location;
        relief->depth = m_transform.Depth;
        relief->rotation = m_transform.Rotation;
        relief->scale = m_transform.Scale;

        if(m_reliefRenderResource.GetCompiledResource().expired()) {
            m_reliefRenderResource.components = Texture2DComponents::R16G16_UINT;
            m_reliefRenderResource.id = m_relief.Get()->GetID();
            m_reliefRenderResource.width = m_reliefData.Get()->GetWidth();
            m_reliefRenderResource.height = m_reliefData.Get()->GetHeight();
            m_reliefRenderResource.data = m_reliefData.Get()->GetTextureData();
            m_reliefRenderResource.isDirty = true;
        }

        relief->texture = &m_reliefRenderResource;  
    }
}
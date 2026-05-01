#include "SpriteActor.h"

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>
#include <Graphics/RenderElements/ReliefSpriteRenderElement.h>

void SpriteActor::RecordRenderView(RenderView &renderView)
{
    if(!m_atlasRecord.IsBound() || !m_albedo.IsBound() || !m_relief.IsBound()
        || !m_albedoData.IsBound()  || !m_reliefData.IsBound()) {
        return;
    }

    std::unique_ptr<RenderObject> ro = renderView.AllocateRenderObject();

    // Opaque Sprite Element
    OpaqueSpriteRenderElement* opaque = ro->CreateRenderElement<OpaqueSpriteRenderElement>();

    opaque->position = m_transform.Location;
    opaque->depth = m_transform.Depth;
    opaque->rotation = m_transform.Rotation;
    opaque->scale = m_transform.Scale;

    Texture2DResource albedoTex;
    albedoTex.components = Texture2DComponents::R8G8B8A8;
    albedoTex.id = m_albedo.Get()->GetID();
    albedoTex.data = m_albedoData.Get()->GetTextureData();

    opaque->texture = albedoTex;

    // Relief Sprite Element
    ReliefSpriteRenderElement* relief = ro->CreateRenderElement<ReliefSpriteRenderElement>();

    relief->position = m_transform.Location;
    relief->depth = m_transform.Depth;
    relief->rotation = m_transform.Rotation;
    relief->scale = m_transform.Scale;

    Texture2DResource reliefTex;
    reliefTex.components = Texture2DComponents::R16G16;
    reliefTex.id = m_relief.Get()->GetID();
    reliefTex.data = m_reliefData.Get()->GetTextureData();

    relief->texture = reliefTex;

    renderView.AddDynamicRenderObject(m_ref.GetID(), std::move(ro));
}
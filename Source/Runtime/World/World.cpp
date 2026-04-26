#include "World.h"

#include <Record/RecordLibrary.h>

void World::SpawnDefaultActors()
{
}

void World::PostInit()
{
}

void World::BeginPlay()
{
}

void World::Tick(float DeltaTime)
{
}

void World::PushInputEvent(InputEvent &input)
{
}

void World::BeginDestroy()
{
    m_worldRef.Release();
}

void World::RecordRenderView(RenderView &view)
{
}

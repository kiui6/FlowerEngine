#include "World.h"

#include <Record/RecordLibrary.h>

#include <ranges>

void World::SpawnDefaultActors()
{
}

std::vector<std::shared_ptr<Actor>> World::GetDynamicActors()
{
    return m_dynamicActors | std::views::values | std::ranges::to<std::vector>();
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

void World::RecordRenderView(RenderView &renderView)
{
    for(auto& [key, value] : m_dynamicActors) {
        value->RecordRenderView(renderView);
    }
}

#include "Engine.h"

#include "EngineDelegates.h"
#include <Log/Log.h>

void Engine::InternalTravel()
{
    // TODO: Unload old world
    if(m_world){
        EngineDelegates::OnWorldUnload.Broadcast(m_world);
        //objectLibrary->UnloadObject(m_world->GetID());
    }

    m_world = m_travelWorld;
    m_world->SpawnDefaultActors();
    m_world->PostInit();

#   ifndef EDITOR
    m_world->BeginPlay();
#   endif
    EngineDelegates::OnWorldLoad.Broadcast(m_world);

    LOGF(Log, LogEngine, "Traveled to world: 0x%08X.", m_world->GetID());

    m_travelWorld = nullptr;
}

void Engine::TravelTo(World *travelWorld)
{
    m_travelWorld = travelWorld;
}

void Engine::Tick(float DeltaTime)
{
    if(m_travelWorld) {
        InternalTravel();
    }

    if(m_world) {
        m_world->Tick(DeltaTime);
    }
}

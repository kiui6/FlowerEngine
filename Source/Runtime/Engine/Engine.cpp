#include "Engine.h"

#include "EngineDelegates.h"
#include <Log/Log.h>

#include <GarbageCollector/GarbageCollector.h>
#include <Record/RecordLibrary.h>

#include <Plugins/PluginManager.h>
#include <Entry/EntryRecord.h>

#include <Platform/PlatformDefines.h>

#include <Debug/Tracer/Tracer.h>

void Engine::InternalTravel()
{
    if(m_world){
        EngineDelegates::OnWorldUnload.Broadcast(m_world.get());
        m_world->BeginDestroy();
        m_world.reset();
        m_GC->RequestGCPass(true);
    }

    m_world = std::move(m_travelWorld);
    m_world->SpawnDefaultActors();
    m_world->PostInit();

#   ifndef EDITOR
    m_world->BeginPlay();
#   endif
    EngineDelegates::OnWorldLoad.Broadcast(m_world.get());

    LOGF(Log, LogEngine, "Traveled to world: \"%s\"", m_world->GetName().c_str());

    m_travelWorld = nullptr;
}

Engine::Engine()
{
    m_GC = GetService<GarbageCollector>();
    if(!m_GC) {
        LOG(Fatal, LogEngine, "Failed to retrive pointer to Garbage Collector service. Is it registered?");
    }
}

void Engine::Initialize()
{
    if constexpr(IS_EDITOR) {
        return;
    }

    // TODO: Plugin order errors shall not crash the game, 
    // instead user should get into a main menu, if EntryRecord is present, where they can change load order
    PluginOrderError pluginOrderError{};
    if (!GetService<PluginManager>()->LoadDefaultPlugins(pluginOrderError)) {
        LOG(Fatal, LogEngine, "Failed to start engine: Critical plugin order error!");
        return;
    }

    RecordPtr<EntryRecord> entryRecord = GetService<RecordLibrary>()->LoadRecord<EntryRecord>(ENTRY_RECORD);
    if(!entryRecord.IsBound()) {
        LOG(Fatal, LogEngine, "Failed to start engine: EntryRecord not found!");
        return;
    }

    LOG(Log, LogEngine, "EntryRecord found");
}

void Engine::TravelTo(std::unique_ptr<World> travelWorld)
{
    m_travelWorld = std::move(travelWorld);
}

void Engine::TravelTo(RecordID travelWorldId)
{
    RecordPtr<WorldRecord> worldRecord = GetService<RecordLibrary>()->LoadRecord<WorldRecord>(travelWorldId);
    if(!worldRecord.IsBound()) {
        LOGF(Error, LogEngine, "Failed to travel to world: ID[0x%016llX] is invalid for WorldRecord", travelWorldId);
        return;
    }

    m_travelWorld = std::make_unique<World>(std::move(worldRecord));
}

void Engine::Tick(float DeltaTime)
{
    PUSH_TRACE_SCOPE("Engine::Tick()");

    if(m_travelWorld) {
        InternalTravel();
    }

    if(m_world) {
        m_world->Tick(DeltaTime);
    }

    EngineDelegates::OnEngineTick.Broadcast(DeltaTime);

    if(m_GC->IsGCPassRequested()) {
        m_GC->RunGCPass(m_GC->IsRequestedGCPassUnrestricted());
    }

    POP_TRACE_SCOPE();
}

void Engine::RecordRenderView(RenderView &renderView)
{
    PUSH_TRACE_SCOPE("Engine::RecordRenderView");
    if(m_world) {
        m_world->RecordRenderView(renderView);
    }
    POP_TRACE_SCOPE();
}

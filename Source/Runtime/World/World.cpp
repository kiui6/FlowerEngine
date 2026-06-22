#include "World.h"

#include <Record/RecordLibrary.h>
#include <Record/RecordFactoryLibrary.h>

#include <Log/Log.h>

#include <ranges>

#include <Graphics/RenderStateUpdates/GlobalStateUpdate.h>

#include "Actor/ActorFactoryLibrary.h"

void World::SpawnDefaultActors()
{
}

Actor *World::SpawnActor(ActorCreateInfo &createInfo)
{
    if(createInfo.base == nullptr) {
        LOG(Assert, LogWorld, "Failed to spawn actor, ActorCreateInfo provided invalid Base record.");
        return nullptr;
    }

    RecordPtr<Record> ref = GetService<RecordLibrary>()->CreateRecordFromType(ReferenceRecord::StaticType());
    if(!ref.IsBound()) {
        LOG(Assert, LogWorld, "Failed to spawn actor, unable to create ReferenceRecord.");
        return nullptr;
    }

    RecordPtr<ReferenceRecord> refptr = CastRecord<ReferenceRecord>(ref);

    refptr->IsDynamic = createInfo.isDynamic;
    refptr->BaseTypename = createInfo.base->GetType();
    refptr->Base = createInfo.base->GetID();
    refptr->DisplayName = createInfo.displayName;

    return InstantiateActor(refptr, createInfo);
}

Actor *World::InstantiateActor(const RecordPtr<ReferenceRecord>& ref, const ActorInstantiateInfo &createInfo)
{
    if(!ref || ref->BaseTypename == 0 || ref->Base == 0) {
        LOG(Error, LogWorld, "Failed to instantiate actor from a ReferenceRecord, insufficient data.");
        return nullptr;
    }

    if(auto existingActor = m_dynamicActors.Find(ref.GetID())) [[unlikely]] {
        return existingActor->get();
    }

    if(auto existingActor = m_staticActors.Find(ref.GetID())) [[unlikely]] {
        return existingActor->get();
    }

    ActorFactory* factory = ActorFactoryLibrary::Get().GetFactory(ref->BaseTypename);
    if(!factory) {
        LOG(Error, LogWorld, "Failed to instantiate actor from a ReferenceRecord, REFR::NAME is specified, but no associated factory is registered.");
        return nullptr;
    }

    std::unique_ptr<Actor> actor = factory->CreateActor(ref);
    Actor* actorPtr = actor.get();
    if(!actor) {
        LOG(Error, LogWorld, "Failed to instantiate actor from a ReferenceRecord, factory is present, but returned nullptr.");
        return nullptr;
    }
    
    actor->PostInit();

    if(ref->IsDynamic) {
        m_dynamicActors.Emplace(ref->GetID(), std::move(actor));
    } else {
        m_staticActors.Emplace(ref->GetID(), std::move(actor));
    }

    return actorPtr;
}

void World::PostInit()
{
}

void World::BeginPlay()
{
}

void World::Tick(float DeltaTime)
{
    for(const auto& [key, actor] : m_dynamicActors) {
        actor->Tick(DeltaTime);
    }
}

void World::Update(float DeltaTime, const InputView &input)
{
    for(const auto& [key, actor] : m_dynamicActors) {
        // Update input and prepare movement & action information for this frame
        actor->OnInput(input);
        // Perform user defined logic
        actor->Tick(DeltaTime);
        // Finally perform simulation updates (movement)
        actor->SimulationUpdate(DeltaTime);
    }
}

void World::ProcessInput(const InputView& input)
{
    for(const auto& [key, actor] : m_dynamicActors) {
        actor->OnInput(input);
    }
}

void World::BeginDestroy()
{
    m_worldRef.Release();
}

void World::RecordRenderView(RenderView &renderView)
{
    renderView.SetCameraPosition({0, 0, 0});

    for(const auto& [key, actor] : m_staticActors) {
        actor->RecordRenderView(renderView);
    }

    for(const auto& [key, actor] : m_dynamicActors) {
        actor->RecordRenderView(renderView);
    }
}

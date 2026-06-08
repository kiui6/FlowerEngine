#include "World.h"

#include <Record/RecordLibrary.h>
#include <Record/RecordFactoryLibrary.h>

#include <Log/Log.h>

#include <ranges>

#include <Graphics/RenderStateUpdates/GlobalStateUpdate.h>

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

    ReferenceRecord* refptr = static_cast<ReferenceRecord*>(ref.Get());

    refptr->IsDynamic = createInfo.isDynamic;
    refptr->BaseTypename = createInfo.base->GetType();
    refptr->Base = createInfo.base->GetID();
    refptr->DisplayName = createInfo.displayName;

    return InstantiateActor(refptr, createInfo);
}

Actor *World::InstantiateActor(ReferenceRecord *ref, ActorInstantiateInfo &createInfo)
{
    if(!ref || ref->BaseTypename == 0 || ref->Base == 0) {
        LOG(Error, LogWorld, "Failed to instantiate actor from a ReferenceRecord, insufficient data.");
        return nullptr;
    }

    RecordFactory* factory = RecordFactoryLibrary::Get().GetFactory(ref->BaseTypename);
    if(!factory) {
        LOG(Error, LogWorld, "Failed to instantiate actor from a ReferenceRecord, REFR::NAME is specified, but no associated factory is registered.");
        return nullptr;
    }

    Actor* actor = factory->CreateActor(ref);
    if(!actor) {
        LOG(Error, LogWorld, "Failed to instantiate actor from a ReferenceRecord, factory is present, but returned nullptr.");
        return nullptr;
    }

    actor->SetReference(ref);

    actor->Initialize();

    if(ref->IsDynamic) {
        m_dynamicActors.emplace(ref->GetID(), std::unique_ptr<Actor>(actor));
    } else {
        m_staticActors.emplace(ref->GetID(), std::unique_ptr<Actor>(actor));
    }

    return actor;
}

std::vector<Actor *> World::GetDynamicActors()
{
    return m_dynamicActors 
        | std::views::values
        | std::views::transform([](auto& ptr) { return ptr.get(); }) 
        | std::ranges::to<std::vector>();
}

void World::PostInit()
{
}

void World::BeginPlay()
{
}

void World::Tick(float DeltaTime)
{
for(auto& [record, actor] : m_dynamicActors) {
        actor->Tick(DeltaTime);
    }
}

void World::ProcessInput(const InputView& input)
{
}

void World::BeginDestroy()
{
    m_worldRef.Release();
}

void World::RecordRenderView(RenderView &renderView)
{
    renderView.SetCameraPosition({0, 0, 0});

    for(auto& [key, value] : m_dynamicActors) {
        value->RecordRenderView(renderView);
    }
}

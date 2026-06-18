#pragma once

#include <Input/InputView.h>

//#include <Object/ObjectRef.h>
#include "Actor/Actor.h"
#include "Chunk/Chunk.h"

#include "WorldRecord.h"

#include <Math/Quadtree.h> 

#include <Utility/Containers/FlatHashMap.h>
#include <Utility/Hash.h>

#include <Graphics/RenderEngine/RenderView/RenderView.h>

#include <vector>
#include <unordered_map>
#include <memory>

class World
{
    FlatHashMap<RecordID, std::unique_ptr<Actor>, GoldHash> m_staticActors;
    FlatHashMap<RecordID, std::unique_ptr<Actor>, GoldHash> m_dynamicActors;

    std::vector<WorldChunk> m_chunks;
    
    RecordPtr<WorldRecord> m_worldRef;

    Quadtree<ActorSpatialInstance> m_quadtree;
public:
    World(const RecordPtr<WorldRecord>& worldRec) 
        : m_worldRef(worldRec)
        {}
    World(RecordPtr<WorldRecord>&& worldRec) : m_worldRef(worldRec) {}

    const std::string& GetName() {return m_worldRef->DisplayName;}

    // Gets called first when world's loaded.
    // Creates Player Controller and other default actors
    void SpawnDefaultActors();

    WeakRecordPtr<WorldRecord> GetRecord() {return m_worldRef;}

    /*
     * Spawns new empty actor of specified type
     * Creates unique ReferenceRecord for it
     * Must provide Base for creation 
     */
    Actor* SpawnActor(ActorCreateInfo& createInfo);
    /*
     * Constructs actor from ReferenceRecord 
     */
    Actor* InstantiateActor(const RecordPtr<ReferenceRecord>& ref, const ActorInstantiateInfo& createInfo);

    const FlatHashMap<RecordID, std::unique_ptr<Actor>, GoldHash>& GetDynamicActors() const {return m_dynamicActors;}

    // TODO: All per-tick functions shall be merged into one 
    // (except RecordRenderView(), which is a special rendering function that must be called after the whole frame ticked)

    // Gets called right after SpawnDefaultActors()
    // Allows actors to initialize their data that should be ready on BeginPlay
    void PostInit();

    void BeginPlay();

    void ProcessInput(const InputView& input);

    void Tick(float DeltaTime);

    void Update(float DeltaTime, const InputView& input);

    void BeginDestroy();

    void RecordRenderView(RenderView& renderView);
};
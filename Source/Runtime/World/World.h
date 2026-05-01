#pragma once

#include <Input/InputEvent.h>

//#include <Object/ObjectRef.h>
#include "Actor/Actor.h"
#include "Chunk/Chunk.h"

#include "WorldRecord.h"

#include <Math/Quadtree.h> 

#include <Graphics/RenderEngine/RenderView/RenderView.h>

#include <vector>
#include <unordered_map>
#include <memory>

class World
{
    std::unordered_map<RecordID, std::unique_ptr<Actor>> m_staticActors;
    std::unordered_map<RecordID, std::unique_ptr<Actor>> m_dynamicActors;
    std::vector<WorldChunk> m_chunks;
    std::string m_name;

    RecordPtr<WorldRecord> m_worldRef;

    Quadtree<ActorSpatialInstance> m_quadtree;
public:

    std::string& GetName() {return m_name;}
    void SetName(std::string_view newName) {m_name = newName;}

    // Gets called first when world's loaded.
    // Creates Player Controller and other default actors
    void SpawnDefaultActors();

    WeakRecordPtr<WorldRecord> GetRecord() {return m_worldRef.GetWeak();}

    /*
     * Spawns new empty actor of specified type
     * Creates unique ReferenceRecord for it
     * Must provide Base for creation 
     */
    Actor* SpawnActor(ActorCreateInfo& createInfo);
    /*
     * Constructs actor from ReferenceRecord 
     */
    Actor* InstantiateActor(ReferenceRecord* ref, ActorInstantiateInfo& createInfo);

    std::vector<Actor*> GetDynamicActors();

    // Gets called right after SpawnDefaultActors()
    // Allows actors to initialize their data that should be ready on BeginPlay
    void PostInit();

    void BeginPlay();

    void Tick(float DeltaTime);

    void PushInputEvent(InputEvent &input);

    void BeginDestroy();

    void RecordRenderView(RenderView& renderView);
};
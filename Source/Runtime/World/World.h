#pragma once

#include <Input/InputEvent.h>

//#include <Object/ObjectRef.h>
#include "Actor.h"
#include "Chunk.h"

#include <Math/Quadtree.h> 

#include <vector>
#include <unordered_map>
#include <memory>

class World
{
    std::unordered_map<RecordID, std::shared_ptr<Actor>> m_staticActors;
    std::unordered_map<RecordID, std::shared_ptr<Actor>> m_dynamicActors;
    std::vector<WorldChunk> m_chunks;
    std::string m_name;

    Quadtree<ActorSpatialInstance> m_quadtree;
public:

    std::string& GetName() {return m_name;}
    void SetName(std::string_view newName) {m_name = newName;}

    // Gets called first when world's loaded.
    // Creates Player Controller and other default entities
    virtual void SpawnDefaultActors();

    // Gets called right after SpawnDefaultEntities()
    // Allows entities to initialize their data that should be ready on BeginPlay
    virtual void PostInit();

    virtual void BeginPlay();

    virtual void Tick(float DeltaTime);

    virtual void PushInputEvent(InputEvent &input);
};
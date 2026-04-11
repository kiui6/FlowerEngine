#pragma once

#include <Input/InputEvent.h>

//#include <Object/ObjectRef.h>
#include "Actor.h"

#include <vector>

class World : public Object
{
    //std::vector<ObjectRef<Actor>> m_actors;
public:

    static std::string GetStaticType() {return "WORLD";}

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
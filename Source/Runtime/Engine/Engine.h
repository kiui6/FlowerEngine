#pragma once

#include <cstdint>

#include <World/World.h>
#include <Blackboard/Blackboard.h>

class Engine
{
    void InternalTravel();
    
protected:
    World* m_world = nullptr;
    World* m_travelWorld = nullptr;

    Blackboard m_blackboard;

public:

#pragma region "World"
    // Starts a process of traveling to the new world
    void TravelTo(World* travelWorld);
    // Loads World for engine and then sets it
    // Only works when called outside of Renderer
    void LoadWorld(uint32_t ID);
    inline World* GetWorld() const {return m_world;}
#pragma endregion "World"

#pragma region "Blackboard"
    Blackboard& GetBlackboard() {return m_blackboard;}
#pragma endregion "Blackboard"

    void Tick(float DeltaTime);
};
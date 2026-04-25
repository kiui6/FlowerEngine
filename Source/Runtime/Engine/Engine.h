#pragma once

#include <cstdint>

#include <World/World.h>

class Engine
{
    void InternalTravel();
    
protected:
    World* m_world = nullptr;
    World* m_travelWorld = nullptr;

    RecordLibrary* m_recLibrary = nullptr;

public:
    Engine();

#pragma region "World"
    // Starts a process of traveling to the new world
    void TravelTo(World* travelWorld);
    // Loads World for engine and then sets it
    // Only works when called outside of Renderer
    void LoadWorld(uint32_t ID);
    inline World* GetWorld() const {return m_world;}
#pragma endregion "World"

    void Tick(float DeltaTime);
};
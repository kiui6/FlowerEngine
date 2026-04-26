#pragma once

#include <cstdint>
#include <memory>

#include <World/World.h>

class Engine
{
    void InternalTravel();
    
protected:
    RecordLibrary* m_recLibrary = nullptr;

    std::unique_ptr<World> m_world = nullptr;
    std::unique_ptr<World> m_travelWorld = nullptr;

    // std::unique_ptr<UINode*> m_uiRootNode = nullptr;

public:
    Engine();

#pragma region "World"
    // Starts a process of traveling to the new world
    void TravelTo(std::unique_ptr<World> travelWorld);
    // Loads World for engine and then sets it
    // Only works when called outside of Renderer
    void LoadWorld(RecordID worldID);
    inline World* GetWorld() const {return m_world.get();}
#pragma endregion "World"

    void Tick(float DeltaTime);

    void RecordRenderView(RenderView& renderView);
};
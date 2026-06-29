#pragma once

#include <cstdint>
#include <memory>

#include <World/World.h>
#include <UI/UINode.h>

#include <Input/InputManager.h>

class GarbageCollector;

class Engine
{
protected:
    GarbageCollector* m_GC = nullptr;

    std::unique_ptr<World> m_world = nullptr;
    std::unique_ptr<World> m_travelWorld = nullptr;

    std::unique_ptr<UINode> m_rootUINode = nullptr;

    uint32_t m_gameCanvasWidth, m_gameCanvasHeight;
    bool m_gameCanvasDirty;

    InputManager m_inputMgr;
public:
    void Initialize();

    // Input API
    void InitializeInputSystem(RawInputDevice* inputDev);
    InputManager& GetInputManager() {return m_inputMgr;}

    /*
     * Loads world record, constructs world out of it and request travel on next frame
     * 
     * @note
     * This function will cause WorldRecord loading.
     * Traveling will be queued and will only be performed on the next tick.
     * 
     * @param travelWorldId
     * an ID of a WorldRecord to use for world construction
     */
    void LoadAndTravel(RecordID travelWorldId);

    /*
     * Constructs world out of world record and request travel on next frame
     * 
     * @note
     * This function will cause WorldRecord loading.
     * Traveling will be queued and will only be performed on the next tick.
     * 
     * @param travelWorldId
     * an ID of a WorldRecord to use for world construction
     */
    void Travel(RecordPtr<WorldRecord> travelWorldId);

    // TODO: Remove or make Editor API
    // Starts a process of traveling to the new world, already existing world
    void TravelToConstructedWorld(std::unique_ptr<World> travelWorld);
    
    // Directly loads into a pre-loaded world
    // Mainly used in editor
    // Only works when called outside of Renderer
    void SetWorld(RecordPtr<WorldRecord> worldID);

    // Loads World for engine and then sets it
    // Only works when called outside of Renderer
    void LoadWorld(RecordID worldID);
    
    inline World* GetWorld() const {return m_world.get();}
    inline bool HasWorld() const {return m_world.get() != nullptr;}

    void Tick(float DeltaTime);

    void RecordRenderView(RenderView& renderView);
private:
    void UnloadCurrentWorld();
    void InternalTravel();
};
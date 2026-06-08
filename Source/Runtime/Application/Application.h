#pragma once

#include <Graphics/RenderEngine/RenderEngine.h>
#include <mutex>
#include <atomic>
#include <barrier>
#include <condition_variable>
#include <functional>
#include <memory>

#include <Log/Log.h>

#include <Window/Window.h>
#include <Engine/Engine.h>
#include <Input/InputManager.h>

#include <Debug/DebugWindow.h>

class Application
{
    static Application* Singleton;
protected:
    Window m_window;

    InputManager m_inputMgr;

    RenderEngine m_render;
    Engine m_engine;

    std::shared_ptr<DebugWindow> m_dbgWindow;
    bool m_dbgWindowUpdated = false;

    std::atomic_bool m_bRunning = false;

    float m_deltaTime = 0;
public:
    Application();
    ~Application();

    static Application* Get() {return Singleton;}

    Window& GetWindow() {return m_window;}
    Engine& GetEngine() {return m_engine;}
    RenderEngine& GetRenderEngine() {return m_render;}

    void SetDebugWindow(DebugWindow* window);

    void Initialize();

    void StartLifecycle();

    void Stop();
protected:
    //void OnWindowEvent(Window* win, WindowEventPayload* payload);
    void RecordRenderStateUpdates(RenderView& view);
};
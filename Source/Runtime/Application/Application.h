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

class Application
{
    static Application* Singleton;
protected:
    std::unique_ptr<Window> m_window;

    std::unique_ptr<RenderEngine> m_render;
    std::unique_ptr<Engine> m_engine;

    std::atomic_bool m_bRunning = false;

    float m_deltaTime = 0;
public:
    Application();
    ~Application();

    static Application* Get() {return Singleton;}

    Engine* GetEngine() {return m_engine.get();}
    RenderEngine* GetRenderEngine() {return m_render.get();}

    void Initialize();

    void StartLifecycle();

    void Stop();
protected:
    //void OnWindowEvent(Window* win, WindowEventPayload* payload);

};
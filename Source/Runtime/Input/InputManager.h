#pragma once

#include <Service/Service.h>

#include <Delegate/Delegate.h>
#include <Window/Window.h>
#include <unordered_map>

#include "InputEvent.h"

/*
 * InGame Input Event Manager
 * Only handles game events, everything aside them can be hooked by Window event handler. 
 */
class InputManager : public IService
{
    static bool isInitialized;
    void InternalHandleEvent(SDL_Event e);
protected:
    bool IsActive = true;
    bool IsGamepad = false;
public:

    static std::string_view GetStaticName() {return "InputManager";}

    void Initialize() override {}

    void SubscribeToWindow(Window* window) {
        window->OnSDLEvent.BindMember(this, &InputManager::InternalHandleEvent);
    }

    /*
    * Multicast delegate, bound functions will be
    * called each time the input event occurs.
    */
    MulticastDelegate<InputEvent> OnInputEvent;
    
};
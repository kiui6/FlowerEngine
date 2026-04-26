#include "InputManager.h"
#include <Log/Log.h>

bool InputManager::isInitialized = RegisterService<InputManager>();

void InputManager::InternalHandleEvent(SDL_Event e) {
    InputEvent event;

    switch(e.type) {
        case SDL_EVENT_KEY_UP:
            event.IsUp = true;
            event.KeyName = SDL_GetKeyName(e.key.key);
            break;
        case SDL_EVENT_KEY_DOWN:
            if(!e.key.repeat) {
                event.KeyName = SDL_GetKeyName(e.key.key);
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            event.IsMouse = true;
            event.AxisX = e.motion.xrel;
            event.AxisY = e.motion.yrel;
            event.MouseX = e.motion.x;
            event.MouseY = e.motion.y;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            event.IsUp = true;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            event.IsMouse = true;
            event.IsKey = true;
            event.MouseButton = e.button.button;
            event.MouseX = e.motion.x;
            event.MouseY = e.motion.y;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            event.IsMouse = true;
            event.IsKey = false;
            event.MouseWheel = e.wheel.y;
            break;
    };

    OnInputEvent.Broadcast(event);
}
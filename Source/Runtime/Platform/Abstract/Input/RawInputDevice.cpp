#include "RawInputDevice.h"
#include <Log/Log.h>

RawInputDevice::RawInputDevice(Window& window)
{
    m_keyboardState = SDL_GetKeyboardState(NULL);
    window.OnSDLEvent.BindMember(this, &RawInputDevice::OnWindowSDLEvent);
}

void RawInputDevice::OnWindowSDLEvent(const SDL_Event &event)
{
    // Engine currently supports only one gamepad
    // For simplicity, only work with first gamepad added

    switch(event.type) {
        case SDL_EVENT_GAMEPAD_ADDED:
            if(m_gamepad) return;
            m_gamepad = SDL_OpenGamepad(event.gdevice.which);
            break;

        case SDL_EVENT_GAMEPAD_REMOVED:
            if(SDL_GetGamepadID(m_gamepad) != event.gdevice.which) return;
            SDL_CloseGamepad(m_gamepad);
            break;

        case SDL_EVENT_MOUSE_WHEEL:
            m_mouseAxis[(uint16_t)InputAxis::Mouse_Wheel_Y] = event.wheel.y;
            break;
    }
}

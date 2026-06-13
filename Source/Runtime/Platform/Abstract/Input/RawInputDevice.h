#pragma once

#include <unordered_map>

#include "InputAction.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>

#include <Platform/Abstract/Window/Window.h>

#include <Log/Log.h>

// TODO: Gamepad state memoization

class RawInputDevice
{
    const bool* m_keyboardState;
    SDL_Gamepad* m_gamepad;

    SDL_MouseButtonFlags m_buttonMask = 0;
    float m_mouseAxis[(uint16_t)InputAxis::Mouse_Axis_Count]{};
public:
    RawInputDevice(Window& window);

    inline void Update() {
        m_buttonMask = SDL_GetRelativeMouseState(&m_mouseAxis[(uint16_t)InputAxis::Mouse_X], &m_mouseAxis[(uint16_t)InputAxis::Mouse_Y]);
    }

    inline void SetGamepad(SDL_Gamepad* gamepad) {m_gamepad = gamepad;}
    inline void ClearGamepad() {m_gamepad = nullptr;}

    inline bool GetKeyState(InputKey key) const {
        const uint16_t keyCode = static_cast<uint16_t>(key);

        // Since we address flat array we must ensure we don't go out of bounds
        if(keyCode < (uint16_t)InputKey::Key_Last_Index) {
            return m_keyboardState[keyCode];
        } else if(keyCode < (uint16_t)InputKey::Mouse_LastIndex) {
            return m_buttonMask & (keyCode - InputKeyMouseOffset);
        } else {
            if(!m_gamepad) return false;
            return SDL_GetGamepadButton(m_gamepad, static_cast<SDL_GamepadButton>(keyCode - InputKeyGamepadOffset));
        }
    }

    inline float GetAxisValue(InputAxis axis) const {
        const uint16_t axisCode = static_cast<uint16_t>(axis);

        // Since we address flat array we must ensure we don't go out of bounds
        if(axisCode < (uint16_t)InputAxis::Mouse_Axis_Count) {
            return m_mouseAxis[axisCode];
        } else {
            if(!m_gamepad) return false;
            constexpr float SDLGamepadAxisCoefficient = 1.f / 32767.f;
            return SDLGamepadAxisCoefficient * SDL_GetGamepadAxis(m_gamepad, static_cast<SDL_GamepadAxis>(axisCode - InputAxisGamepadOffset));
        }
    }
private:
    void OnWindowSDLEvent(const SDL_Event& event);
};
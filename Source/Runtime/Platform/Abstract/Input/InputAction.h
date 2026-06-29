#pragma once
#include <string>
#include <cstdint>

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>

// 512 is the scancode count in SDL, but enum value isn't used to avoid damaging input configurations in games after engine updates
// If scancode count gets changed, extra range is not supported
constexpr uint16_t InputKeyMouseOffset = 512;
// for faster math, gamepad offset starts from the next power of two value
constexpr uint16_t InputKeyGamepadOffset = 1024;

enum class InputKey : uint16_t {
    // TODO: Fill all scancodes

    Key_A = SDL_SCANCODE_A,
    Key_B = SDL_SCANCODE_B,
    Key_C = SDL_SCANCODE_C,
    Key_D = SDL_SCANCODE_D,
    Key_E = SDL_SCANCODE_E,
    Key_F = SDL_SCANCODE_F,
    Key_G = SDL_SCANCODE_G,
    Key_H = SDL_SCANCODE_H,
    Key_I = SDL_SCANCODE_I,
    Key_J = SDL_SCANCODE_J,
    Key_K = SDL_SCANCODE_K,
    Key_L = SDL_SCANCODE_L,
    Key_M = SDL_SCANCODE_M,
    Key_N = SDL_SCANCODE_N,
    Key_O = SDL_SCANCODE_O,
    Key_P = SDL_SCANCODE_P,
    Key_Q = SDL_SCANCODE_Q,
    Key_R = SDL_SCANCODE_R,
    Key_S = SDL_SCANCODE_S,
    Key_T = SDL_SCANCODE_T,
    Key_U = SDL_SCANCODE_U,
    Key_V = SDL_SCANCODE_V,
    Key_W = SDL_SCANCODE_W,
    Key_X = SDL_SCANCODE_X,
    Key_Y = SDL_SCANCODE_Y,
    Key_Z = SDL_SCANCODE_Z,

    Key_1 = SDL_SCANCODE_1,
    Key_2 = SDL_SCANCODE_2,
    Key_3 = SDL_SCANCODE_3,
    Key_4 = SDL_SCANCODE_4,
    Key_5 = SDL_SCANCODE_5,
    Key_6 = SDL_SCANCODE_6,
    Key_7 = SDL_SCANCODE_7,
    Key_8 = SDL_SCANCODE_8,
    Key_9 = SDL_SCANCODE_9,
    Key_0 = SDL_SCANCODE_0,

    Key_Last_Index = SDL_SCANCODE_COUNT,

    Mouse_Left = InputKeyMouseOffset + SDL_BUTTON_LMASK,
    Mouse_Middle = InputKeyMouseOffset + SDL_BUTTON_MMASK,
    Mouse_Right = InputKeyMouseOffset + SDL_BUTTON_RMASK,
    Mouse_3 = InputKeyMouseOffset + SDL_BUTTON_X1MASK,
    Mouse_4 = InputKeyMouseOffset + SDL_BUTTON_X2MASK,
    Mouse_5 = InputKeyMouseOffset + SDL_BUTTON_MASK(6),
    Mouse_6 = InputKeyMouseOffset + SDL_BUTTON_MASK(7),
    Mouse_LastIndex,

    // Gamepad Buttons at the right

    Gamepad_Button_Up = InputKeyGamepadOffset + SDL_GAMEPAD_BUTTON_SOUTH,
    Gamepad_Button_Right = InputKeyGamepadOffset + SDL_GAMEPAD_BUTTON_EAST,
    Gamepad_Button_Down = InputKeyGamepadOffset + SDL_GAMEPAD_BUTTON_WEST,
    Gamepad_Button_Left = InputKeyGamepadOffset + SDL_GAMEPAD_BUTTON_NORTH,

    // Gamepad Buttons at the left pad

    Gamepad_Pad_Down,
    Gamepad_Pad_Right,
    Gamepad_Pad_Left,
    Gamepad_Pad_Up,


    // Gamepad Triggers

    Gamepad_L1,
    Gamepad_L2,
    Gamepad_L3,
    Gamepad_R1,
    Gamepad_R2,
    Gamepad_R3,

    // Gamepad service buttons

    Gamepad_Start,
    Gamepad_Menu,
};

enum class InputKeyState : uint8_t {
    None = 0,
    JustPressed = 1,
    Held = 2,
    JustReleased = 3
};

enum class InputModKey : uint8_t {
    None    = 0, 
    Shift   = 1 << 0,
    Ctrl    = 1 << 1,
    Alt     = 1 << 2,
    Super   = 1 << 3,
};

constexpr uint16_t InputAxisGamepadOffset = 512;

enum class InputAxis : uint16_t {
    Mouse_X,
    Mouse_Y,
    Mouse_Wheel_X,
    Mouse_Wheel_Y,
    Mouse_Axis_Count,

    Gamepad_Stick_Right_X = InputAxisGamepadOffset,
    Gamepad_Stick_Right_Y,
    Gamepad_Stick_Left_X,
    Gamepad_Stick_Left_Y,

    Gamepad_L2,
    Gamepad_R2,
};

struct InputAction {
    InputKey key;
    InputKeyState state;
    InputModKey mod;
};
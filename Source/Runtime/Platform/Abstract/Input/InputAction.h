#pragma once
#include <string>
#include <cstdint>

#include <SDL3/SDL.h>

enum class InputKey : uint16_t {
    Key_A = SDL_SCANCODE_A,
    Key_B = SDL_SCANCODE_B,

    Mouse_Left = 1024,
    Mouse_Middle,
    Mouse_Right,
    Mouse_3,
    Mouse_4,
    Mouse_5,


    // Gamepad Buttons at the left pad

    Gamepad_Pad_Up = 2048,
    Gamepad_Pad_Right,
    Gamepad_Pad_Down,
    Gamepad_Pad_Left,

    // Gamepad Buttons at the right

    Gamepad_Button_Up,
    Gamepad_Button_Right,
    Gamepad_Button_Down,
    Gamepad_Button_Left,

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
    Pressed,
    Released
};

enum class InputModKey : uint8_t {
    None    = 0, 
    Shift   = 1 << 0,
    Ctrl    = 1 << 1,
    Alt     = 1 << 2,
    Super   = 1 << 3,
};

enum class InputAxis : uint16_t {
    Mouse_X,
    Mouse_Y,
    Mouse_Wheel_X,
    Mouse_Wheel_Y,

    Gamepad_Stick_Right_X,
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
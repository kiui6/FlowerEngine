#pragma once

#include <Delegate/Delegate.h>

class World;
struct InputEvent;

class EngineDelegates
{
public:
    static MulticastDelegate<World*> OnWorldLoad;
    static MulticastDelegate<World*> OnWorldUnload;

    static MulticastDelegate<float> OnEngineTick;

    static MulticastDelegate<InputEvent*> OnInputRaw;
};
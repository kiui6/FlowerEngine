#pragma once

#include <Engine/EngineDelegates.h>

struct Tickable
{
public:
    Tickable() {EngineDelegates::OnEngineTick.BindMember(this, &Tickable::Tick);}
    ~Tickable() = default;

    virtual void Tick(float deltaTime) = 0;
};
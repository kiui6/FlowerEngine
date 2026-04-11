#pragma once

#include "EngineDelegates.h"

#include <vector>

struct ITickable;

class TickableManager
{
    std::vector<ITickable*> TickableObjects;
public:

    void RegisterTickable(ITickable* tickable);
    void UnregisterTickable(ITickable* tickable); 
};

struct ITickable
{
public:
    ITickable() {EngineDelegates::OnEngineTick.BindMember(this, &ITickable::Tick);}
    ~ITickable() {}

    virtual void Tick(float deltaTime) {}
};
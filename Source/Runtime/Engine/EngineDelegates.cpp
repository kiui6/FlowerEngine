#include "EngineDelegates.h"

MulticastDelegate<World*> EngineDelegates::OnWorldLoad;
MulticastDelegate<World*> EngineDelegates::OnWorldUnload;

MulticastDelegate<float> EngineDelegates::OnEngineTick;
#include "Tickable.h"

void TickableManager::RegisterTickable(ITickable *tickable)
{
    TickableObjects.emplace_back(tickable);
}

void TickableManager::UnregisterTickable(ITickable *tickable)
{
    for(auto it = TickableObjects.begin(); it != TickableObjects.end(); ++it)
    {
        if(*it == tickable)
        {
            TickableObjects.erase(it);
            return;
        }
    }
}

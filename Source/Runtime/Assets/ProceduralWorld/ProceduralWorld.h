#pragma once

#include <World/World.h>
#include <Assets/Entity/EntityFactory.h>

class ProceduralWorld : public World
{
    EntityFactory m_entityFactory;
};
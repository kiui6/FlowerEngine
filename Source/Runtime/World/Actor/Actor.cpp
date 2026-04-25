#include "Actor.h"

#include <World/World.h>

World *Actor::GetWorld()
{
    return m_world;
}

void Actor::Tick(float deltaTime)
{
}

Transform2D Actor::GetTransform()
{
    return transform;
}

glm::vec2 Actor::GetLocation()
{
    return transform.Location;
}

void Actor::SetLocation(const glm::vec2 &loc)
{
    transform.Location = loc;
}

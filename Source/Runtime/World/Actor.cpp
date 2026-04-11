#include "Actor.h"

#include <World/World.h>

World *Actor::GetWorld()
{
    return GetParent<World>();
}

void Actor::Tick(float deltaTime)
{
}

Transform Actor::GetTransform()
{
    if(Actor* parent = GetParent<Actor>()) {
        return transform + parent->transform;
    } else {
        return transform;
    }
}

glm::vec3 Actor::GetLocation()
{
    if(Actor* act = GetParent<Actor>()) {
        return transform.Location + act->transform.Location;
    } else {
        return transform.Location;
    }
}

void Actor::SetLocation(const glm::vec3 &loc)
{
    if(Actor* act = GetParent<Actor>()) {
        transform.Location += loc - act->transform.Location;
    } else {
        transform.Location = loc;
    }
}

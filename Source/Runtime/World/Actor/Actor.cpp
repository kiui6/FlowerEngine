#include "Actor.h"

#include <World/World.h>

#include <Record/RecordLibrary.h>

Actor::Actor(const RecordPtr<ReferenceRecord> &reference)
    : m_ref(reference)
{
    m_transform.Location.x = m_ref->Transform.Get().LocationX();
    m_transform.Location.y = m_ref->Transform.Get().LocationY();
    m_transform.Pivot.x = m_ref->Transform.Get().PivotX();
    m_transform.Pivot.y = m_ref->Transform.Get().PivotY();
    m_transform.Rotation = m_ref->Transform.Get().Rotation();
    m_transform.Depth = m_ref->Transform.Get().Depth();
}

World *Actor::GetWorld()
{
    return m_world;
}

void Actor::Tick(float deltaTime)
{
}

Transform2D Actor::GetTransform()
{
    return m_transform;
}

glm::vec2 Actor::GetLocation()
{
    return m_transform.Location;
}

void Actor::SetLocation(const glm::vec2 &loc)
{
    m_transform.Location = loc;
}

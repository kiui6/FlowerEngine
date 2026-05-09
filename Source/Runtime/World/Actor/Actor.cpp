#include "Actor.h"

#include <World/World.h>

#include <Record/RecordLibrary.h>

void Actor::SetReference(ReferenceRecord *ref)
{
    m_ref = ref;

    DisplayName = m_ref->DisplayName;
}

void Actor::SetReferenceByID(RecordID recId)
{
    m_ref = GetService<RecordLibrary>()->LoadRecord<ReferenceRecord>(recId);
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

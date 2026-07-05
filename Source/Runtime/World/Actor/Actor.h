#pragma once

#include <Record/Record.h>
#include <Record/RecordPtr.h>
#include <World/Actor/ReferenceRecord.h>

#include <Input/InputView.h>

#include <Math/Transform.h>
#include <Math/Bounds.h>

#include <Graphics/RenderEngine/RenderView/RenderView.h>

#include <Localization/Text.h>

#include <Delegate/Delegate.h>

#include <memory>

struct ActorSpatialInstance {
    AABB bounds;
    Float2 location;
    RecordID refid;
    bool isDynamic;
};

class Actor;

template<typename T>
concept ActorClass = std::is_base_of_v<Actor, T>;

enum class ActorFlags : uint8_t {
    // This flag means that Actor will get destroyed next tick
    // It will also remove actor from Record Library. 
    BeginDestroy = (1 << 0),
};

class Actor
{
protected:
    RecordPtr<ReferenceRecord> m_ref;

    Transform2D m_transform;

    uint8_t actorFlags = 0;

    Text DisplayName = {"NoName"};

    World* m_world;
public:
    Actor() = delete;
    Actor(const RecordPtr<ReferenceRecord>& reference);
    virtual ~Actor() = default;

    static uint32_t RecordType() {return FIELDID("ACT_");}

    inline ReferenceRecord* GetReference() const {return m_ref.Get();}
    inline RecordID GetReferenceID() const {return m_ref.GetID();}

    virtual World* GetWorld();

    virtual void PostInit(){}
    virtual void BeginPlay() {}

    virtual void OnInput(const InputView& input) {}
    virtual void Tick(float deltaTime);
    // Performs final simulation update after input & tick for current frame
    virtual void SimulationUpdate(float deltaTime) {}

    virtual void SetTransform(const Transform2D& transform) { m_transform = transform; }
    virtual Transform2D GetTransform();
    virtual const Transform2D& GetRelativeTransform() const {return m_transform;}

    virtual glm::vec2 GetLocation();
    virtual const glm::vec2 GetRelativeLocation() const {return m_transform.Location;}

    virtual void SetLocation(const glm::vec2& loc);
    virtual void SetRelativeLocation(const glm::vec2 loc) {m_transform.Location = loc;}

    inline void SetActorFlag(ActorFlags flag) {actorFlags |= (uint8_t)flag;}
    inline void ClearActorFlag(ActorFlags flag) {actorFlags &= ~(uint8_t)flag;}
    inline bool HasActorFlag(ActorFlags flag) {return (actorFlags & (uint8_t)flag) == (uint8_t)flag;}

    virtual void RecordRenderView(RenderView& renderView) {}

    void BeginDestroy() {SetActorFlag(ActorFlags::BeginDestroy);}

    // Used for testing of collisions and visibility
    virtual AABB GetBoundingBox() {return {};}
protected:
    MulticastDelegate<> m_onCollision{};
    MulticastDelegate<> m_onEnterCollision{};
    MulticastDelegate<> m_onLeaveCollision{};
};

struct ActorInstantiateInfo
{
    Transform2D transform;
};

struct ActorCreateInfo : public ActorInstantiateInfo
{
    bool isDynamic = false;
    Record* base = nullptr;
    Text displayName;
};
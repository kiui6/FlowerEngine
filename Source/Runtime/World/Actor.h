#pragma once

#include <Object/Object.h>

#include <Math/Transform.h>

class Actor;

template<typename T>
concept ActorClass = std::is_base_of_v<Actor, T>;

enum class ActorMobilityPolicy : bool
{
    Static = 0,
    Dynamic = 1
};

enum class ActorFlags : uint8_t {
    // This flag means that Actor will get destroyed next tick
    // It will also remove actor from Object Library. 
    BeginDestroy = (1 << 0),
};

class Actor : public Object
{
protected:
    Transform transform;

    // Dynamic actors are testing against dynamic value, like collision
    ActorMobilityPolicy mobilityPolicy = ActorMobilityPolicy::Static;

    uint8_t actorFlags = 0;

    std::string DisplayName = "Actor";
public:
    inline ActorMobilityPolicy GetMobility() const {return mobilityPolicy;}
    inline void SetMobility(ActorMobilityPolicy newValue) {mobilityPolicy = newValue;}

    static std::string GetStaticType() {return "ACT";}

    virtual World* GetWorld();

    virtual void PostInit(){}
    virtual void BeginPlay() {}
    virtual void Tick(float deltaTime);

    virtual Transform GetTransform();
    virtual const Transform& GetRelativeTransform() const {return transform;}

    virtual glm::vec3 GetLocation();
    virtual const glm::vec3& GetRelativeLocation() const {return transform.Location;}

    virtual void SetLocation(const glm::vec3& loc);
    virtual void SetRelativeLocation(const glm::vec3 loc) {transform.Location = loc;}

    inline void SetActorFlag(ActorFlags flag) {actorFlags |= (uint8_t)flag;}
    inline void ClearActorFlag(ActorFlags flag) {actorFlags &= ~(uint8_t)flag;}
    inline bool HasActorFlag(ActorFlags flag) {return (actorFlags & (uint8_t)flag) == (uint8_t)flag;}

    void BeginDestroy() {SetActorFlag(ActorFlags::BeginDestroy);}

    // Used for testing of collisions and visibility
    virtual AABB GetBoundingBox() {return {};}
};
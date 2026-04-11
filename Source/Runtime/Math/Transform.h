#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct AABB
{
    glm::vec3 AA = {0, 0, 0};
    glm::vec3 BB = {0, 0, 0};
};

struct Transform
{
    glm::vec3 Location = {0, 0, 0};
    glm::vec3 Pivot = {0, 0, 0};
    glm::vec3 Scale = {1, 1, 1};
    glm::quat Rotator;

    Transform(){}

    Transform operator+(Transform const& other) {
        Transform ret;
        ret.Location = Location + other.Location;
        ret.Pivot = Pivot + other.Pivot;
        ret.Scale = Scale + other.Scale;
        //ret.Rotator = Rotator + other.Rotator;
        return ret;
    }

    Transform operator-(Transform const& other) {
        Transform ret;
        ret.Location = Location - other.Location;
        ret.Pivot = Pivot - other.Pivot;
        ret.Scale = Scale - other.Scale;
        //ret.Rotator = Rotator + other.Rotator;
        return ret;
    }
};
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Vectors.h"

struct Transform2D
{
    uint8_t Depth = 0;
    Int2 Location = {0, 0};
    Int2 Pivot = {0, 0};
    Float2 Scale = {1, 1};
    float Rotation;

    Transform2D operator+(Transform2D const& other) {
        Transform2D ret;
        ret.Location = Location + other.Location;
        ret.Pivot = Pivot + other.Pivot;
        ret.Scale = Scale + other.Scale;
        return ret;
    }

    Transform2D operator-(Transform2D const& other) {
        Transform2D ret;
        ret.Location = Location - other.Location;
        ret.Pivot = Pivot - other.Pivot;
        ret.Scale = Scale - other.Scale;
        return ret;
    }
};
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct SceneView
{
    glm::mat4 ProjectionMatrix;
    glm::mat4 InvProjectionMatrix;

    glm::mat4 ViewMatrix;
    glm::mat4 InvViewMatrix;

    glm::mat4 ViewProjectionMatrix;
    glm::mat4 InvViewProjectionMatrix;

    // Camera
    glm::vec3 ViewOrigin;
    glm::quat ViewRotation;
    float FOV = 75;

    // Updates VP Matrix using external precalculated matrix
    void UpdateViewProjectionMatrix(glm::mat4 vpm);
    // Updates VP matrix using local data
    void UpdateViewProjectionMatrix();
    void UpdateViewMatrix(glm::mat4 vm);
    void UpdateProjectionMatrix(glm::mat4 pm);
};
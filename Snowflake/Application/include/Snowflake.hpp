#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <RenderObject.hpp>
#include <Mesh.hpp>
#include <Material.hpp>

/*
 * TODO: Inherit RenderObject class to implement animation
 */

class Snowflake : public Engine::RenderObject {
public:
    Snowflake(Engine::Mesh* mesh, Engine::Material* material, float verticalSpeed, float horizontalSpeed,
            float startPositionX, float startPositionY);

    float _previousElapsedTime = 0;
    float _verticalSpeed;
    float _horizontalSpeed;
    float _startPositionX;
    float _startPositionY;
    // Add more functions
};
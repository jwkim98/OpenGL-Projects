#include <Snowflake.hpp>

/*
 * TODO: Inherit RenderObject class to implement animation
 */

Snowflake::Snowflake(Engine::Mesh* mesh, Engine::Material* material, float verticalSpeed, float horizontalSpeed,
        float startPositionX, float startPositionY)
        :
        Engine::RenderObject(mesh, material), _verticalSpeed(verticalSpeed), _horizontalSpeed(horizontalSpeed),
        _startPositionX(startPositionX), _startPositionY(startPositionY)
{
}

// Add more functions
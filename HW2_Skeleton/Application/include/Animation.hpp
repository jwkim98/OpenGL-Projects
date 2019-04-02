#pragma once

#include <vector>
#include <RenderObject.hpp>

/*
 * Animation class supports continuous animation by changing RenderObject's transform with elasped time in seconds
 */

class Animation
{
private:
    std::vector<Engine::RenderObject*> _objects;
	std::vector<Engine::RenderObject*> _snowmanParts;
public:
    Animation();
    ~Animation();

    void AddObject(Engine::RenderObject* object);
    void Animate(Engine::Camera* cam, float deltaTime);
	void AnimateSnowman(Engine::Camera* cam, float deltaTime);
	
	void AddObjectToSnowman(Engine::RenderObject* object);
};
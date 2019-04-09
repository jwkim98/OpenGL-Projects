#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <PickableObject.hpp>
#include "Geometry.hpp"
#include "DefaultMaterial.hpp"

using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;
class Snowman
{
private:
	float _totalTime = 0;
	vec3 _initialPos;

	DefaultMaterial material;
	Geometry geometry;
	PickingMaterial picking_material;
	Engine::Mesh  sphere_mesh1;
	Engine::Mesh  sphere_mesh2;
	Engine::Mesh cone_mesh;
	Engine::Mesh cylinder_mesh;

	PickableObject _body;
	PickableObject _head;
	PickableObject _rightArm;
	PickableObject _leftArm;
	PickableObject _nose;
	double _armPosition = 0;
	const double _armThreshold = 30;
	bool _armDirection = true;
	double _nosePosition = 0;
	const double _noseThreshold = 30;
	bool _noseDirection = true;


public:

    Snowman(int headIndex, int bodyIndex);
    ~Snowman();
	void RotateHead();
	void RotateBody(float speed);
	void RotateHead(double x_angle, double y_angle);
	void MoveArm(double speed, float elapsed_time);
	void MoveNose(double speed, float elapsed_time);
	void Render(Engine::Camera* camera);
	void RenderPicking(Engine::Camera* camera);
	void SetPosition(vec3 position);
	void Move(vec3 position);
	bool Jump(float elapsed_time, float velocity, bool start);
	vec3 GetLocation();

};
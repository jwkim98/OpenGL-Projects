#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <PickableObject.hpp>

using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;
class Snowman
{
private:
	double _armPosition = 0;
	const double _armThreshold = 30;
	bool _armDirection = true;
	double _nosePosition = 0;
	const double _noseThreshold = 30;
	bool _noseDirection = true;


public:
    Snowman();
    ~Snowman();
	PickableObject* Body;
	PickableObject* Head;
	PickableObject* RightArm;
	PickableObject* LeftArm;
	PickableObject* Nose;
	void rotate_head();
	void rotate_body();
	void rotate_head(double x_angle, double y_angle);
	void move_arm(double speed, float elapsed_time);
	void move_nose(double speed, float elapsed_time);
};
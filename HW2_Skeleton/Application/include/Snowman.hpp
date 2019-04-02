#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <PickableObject.hpp>

class Snowman
{
private:

public:
    Snowman();
    ~Snowman();
	PickableObject* body;
	PickableObject* head;
	void rotate_head();
	void rotate_body();
	void rotate_head(double x_angle, double y_angle);
};
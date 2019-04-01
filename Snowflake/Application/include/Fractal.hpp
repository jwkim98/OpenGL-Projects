#pragma once

#include <vector>
#include <deque>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Mesh.hpp>

/*
 * Implement Fractal class to generate Koch snowflakes mesh (generate snowflake mesh using triangle list).
 * Koch snowflake: https://en.wikipedia.org/wiki/Koch_snowflake
 */
class Fractal
{
private:
	const double PI = 3.14159265;
	std::deque<glm::vec3> _vertexList;
	std::deque<glm::vec3> _colorList;
	int _recursionDepth;
	Engine::Mesh* _mesh;

	void makeTriangleRecursion(int depth, glm::vec3 pointA ,glm::vec3 pointB, double angle);
public:
	void resetColorList();
	void makeFractal();
	void addColor(glm::vec3 color);
    Fractal(int recursionDepth);
    void GenerateSnowflake(Engine::Mesh* mesh, float length, float positionX, float positionY);
};
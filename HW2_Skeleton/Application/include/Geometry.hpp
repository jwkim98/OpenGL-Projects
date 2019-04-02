#pragma once

#include <vector>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Mesh.hpp>

using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;

class Geometry
{
private:

protected:
	Geometry *parent;
	std::vector<Geometry *> children;
public:
	Geometry();
	~Geometry();
	void GenerateCube(Engine::Mesh* mesh);
	void Quad(Engine::Mesh* mesh, int a_a, int a_b, int a_c, int a_d);
	glm::vec3 ComputeNormal(glm::vec4 a_a, glm::vec4 a_b, glm::vec4 a_c);
	void GenerateTriangleMesh(Engine::Mesh* mesh);
	void GenerateLine(Engine::Mesh* mesh);
	void GenerateStar(Engine::Mesh* mesh);

	void Geometry::GenerateSphere(Engine::Mesh* mesh, double radius);
	void Geometry::GenerateCone(Engine::Mesh* mesh, double radius, double height);
	void Geometry::GenerateCylinder(Engine::Mesh* mesh, double radius, double height);
	void Geometry::Triple(Engine::Mesh* mesh, vec4 vec_a, vec4 vec_b, vec4 vec_c);

	void AddChild(Geometry *geometry);
};


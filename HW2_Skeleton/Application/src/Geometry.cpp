#include <Geometry.hpp>
#include <stdlib.h>
#include <cmath>
#include <deque>

Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

glm::vec4 vertices[8] = {
    glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f),
    glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),
    glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),
    glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),
    glm::vec4(1.0f, 1.0f, -1.0f, 1.0f),
    glm::vec4(1.0f, -1.0f, -1.0f, 1.0f)
};

glm::vec3 Geometry::ComputeNormal(glm::vec4 a_a, glm::vec4 a_b, glm::vec4 a_c)
{
    glm::vec3 normal = glm::normalize(glm::cross(a_b.xyz() - a_a.xyz(), a_c.xyz() - a_a.xyz()));
    return normal;
}

// TODO: Fill up Geometry::Quad (Slide No. 9)
void Geometry::Quad(Engine::Mesh* mesh, int a_a, int a_b, int a_c, int a_d)
{
    /* a -- d
     * |    |
     * b -- c
     */
    // Compute normal for quad
    glm::vec4 n = glm::vec4(ComputeNormal(vertices[a_a], vertices[a_b], vertices[a_c]), 0.0f);
    // Triangle 1, Vertex a
    mesh->AddVertexData(vertices[a_a]);
    mesh->AddVertexData(n);
    // Triangle 1, Vertex b
    mesh->AddVertexData(vertices[a_b]);
    mesh->AddVertexData(n);
    // Triangle 1, Vertex c
    mesh->AddVertexData(vertices[a_c]);
    mesh->AddVertexData(n);
    // Triangle 2, Vertex a
    mesh->AddVertexData(vertices[a_a]);
    mesh->AddVertexData(n);
    // Triangle 2, Vertex c
    mesh->AddVertexData(vertices[a_c]);
    mesh->AddVertexData(n);
    // Triangle 2, Vertex d
    mesh->AddVertexData(vertices[a_d]);
    mesh->AddVertexData(n);
}

void Geometry::Triple(Engine::Mesh* mesh, vec4 vec_a, vec4 vec_b, vec4 vec_c)
{
	// Compute normal for quad
glm::vec4 n = glm::vec4(ComputeNormal(vec_a, vec_b, vec_c), 0.0f);
	//glm::vec4 n = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	// Triangle 1, Vertex a
	mesh->AddVertexData(vec_a);
	mesh->AddVertexData(n);
	// Triangle 1, Vertex b
	mesh->AddVertexData(vec_b);
	mesh->AddVertexData(n);
	// 
	mesh->AddVertexData(vec_c);
	mesh->AddVertexData(n);
	
}

void Geometry::GenerateSphere(Engine::Mesh* mesh, double radius)
{
	mesh->AddAttribute(4);
	mesh->AddAttribute(4);
	double PI = glm::pi<double>();

	double interval = PI / 60.0;
	double y_step = 0.01;
	double x_step = 0.01;


	std::deque<vec4> position_vector;
	int elements = 0;

	for(double y_angle = -PI/2 ; y_angle <= PI/2; y_angle += y_step)
	{
		double tempRadius= radius * cos(y_angle); //radius of circle
		double tempRadius_previous = radius * cos(y_angle - y_step);
		double z_pos = radius * sin(y_angle); // position of z axis
		double z_pos_previous = radius * sin(y_angle - y_step);

		for(double x_angle = 0; x_angle <= 2*PI; x_angle += x_step)
		{
			double x_pos= tempRadius * cos(x_angle);
			double y_pos = tempRadius * sin(x_angle);
			double x_pos_previous = tempRadius * cos(x_angle - x_step);
			double y_pos_previous = tempRadius * sin(x_angle - x_step);

			double x_pos_down = tempRadius_previous * cos(x_angle);
			double y_pos_down = tempRadius_previous * sin(x_angle);
			double x_pos_previous_down = tempRadius_previous * cos(x_angle - x_step);
			double y_pos_previous_down = tempRadius_previous * sin(x_angle - x_step);

			vec4 pos1 = vec4(x_pos_previous_down, y_pos_previous_down, z_pos_previous, 1);
			vec4 pos2 = vec4(x_pos_down, y_pos_down, z_pos_previous, 1);
			vec4 pos3 = vec4(x_pos, y_pos, z_pos, 1);
			vec4 pos4 = vec4(x_pos_previous, y_pos_previous, z_pos, 1);

			Triple(mesh, pos1, pos2, pos4);
			Triple(mesh, pos2, pos3, pos4);

			elements += 6;
		}
	}
	mesh->SetNumElements(elements);
	mesh->CreateMesh();
	return;
}

void Geometry::GenerateCone(Engine::Mesh* mesh, double radius, double height)
{
	mesh->AddAttribute(4);
	mesh->AddAttribute(4);

	double precision = 0.05;
	double height_precision = 0.05;

	double num_elements = 0;
	vec4 base = vec4(0, 0, 0, 1);

	for (double angle = 0; angle < 2 * glm::pi<double>(); angle += precision)
	{
		double x_pre = radius * cos(angle - precision);
		double x = radius * cos(angle);

		double y_pre = radius * sin(angle - precision);
		double y = radius * sin(angle);

		vec4 pos_1 = vec4(x_pre, y_pre, 0, 1);
		vec4 pos_2 = vec4(x, y, 0, 1);

		Triple(mesh, pos_1, pos_2, base);
		num_elements += 3;
	}

	for(double h = height_precision; h < height ; h+=height_precision)
	{
		double current_height = h;
		double previous_height = h - height_precision;

		double current_radius = (radius / height) * (height - current_height);
		double previous_radius = (radius / height) * (height - previous_height);

		for(double angle = 0; angle < 2*glm::pi<double>(); angle += precision)
		{
			double x_pre_down = previous_radius * cos(angle - precision);
			double x_down = previous_radius * cos(angle);
			double x_pre_up = current_radius * cos(angle - precision);
			double x_up = current_radius * cos(angle);

			double y_pre_down = previous_radius * sin(angle - precision);
			double y_down = previous_radius * sin(angle);
			double y_pre_up = current_radius * sin(angle - precision);
			double y_up = current_radius * sin(angle);

			vec4 pos_1 = vec4(x_pre_down, y_pre_down, previous_height, 1);
			vec4 pos_2 = vec4(x_down, y_down, previous_height, 1);
			vec4 pos_3 = vec4(x_up, y_up, current_height, 1);
			vec4 pos_4 = vec4(x_pre_up, y_pre_up, current_height, 1);

			Triple(mesh, pos_1, pos_2, pos_4);
			Triple(mesh, pos_2, pos_3, pos_4);

			num_elements += 6;
		}
	}
	mesh->SetNumElements(num_elements);
	mesh->CreateMesh();
}

void Geometry::GenerateCylinder(Engine::Mesh* mesh, double radius, double height)
{
	mesh->AddAttribute(4);
	mesh->AddAttribute(4);

	double precision = 0.05;
	double height_precision = 0.05;

	double num_elements = 0;

	vec4 base_lower = vec4(0, 0, 0, 1);

	for (double angle = 0; angle < 2 * glm::pi<double>(); angle += precision)
	{
		double x_pre = radius * cos(angle - precision);
		double x = radius * cos(angle);

		double y_pre = radius * sin(angle - precision);
		double y = radius * sin(angle);

		vec4 pos_1 = vec4(x_pre, y_pre, 0, 1);
		vec4 pos_2 = vec4(x, y, 0, 1);

		Triple(mesh, pos_1, pos_2, base_lower);
		num_elements += 3;
	}

	vec4 base_upper = vec4(0, 0, height, 1);

	for (double angle = 0; angle < 2 * glm::pi<double>(); angle += precision)
	{
		double x_pre = radius * cos(angle - precision);
		double x = radius * cos(angle);

		double y_pre = radius * sin(angle - precision);
		double y = radius * sin(angle);

		vec4 pos_1 = vec4(x_pre, y_pre, height, 1);
		vec4 pos_2 = vec4(x, y, height, 1);

		Triple(mesh, pos_1, pos_2, base_upper);
		num_elements += 3;
	}

	for (double h = 0; h < height; h += height_precision)
	{
		double current_height = h;
		double previous_height = h - height_precision;


		for (double angle = 0; angle < 2 * glm::pi<double>(); angle += precision)
		{
			double x_pre_down = radius * cos(angle - precision);
			double x_down = radius * cos(angle);
			double x_pre_up = radius * cos(angle - precision);
			double x_up = radius * cos(angle);

			double y_pre_down = radius * sin(angle - precision);
			double y_down = radius * sin(angle);
			double y_pre_up = radius * sin(angle - precision);
			double y_up = radius * sin(angle);

			vec4 pos_1 = vec4(x_pre_down, y_pre_down, previous_height, 1);
			vec4 pos_2 = vec4(x_down, y_down, previous_height, 1);
			vec4 pos_3 = vec4(x_up, y_up, current_height, 1);
			vec4 pos_4 = vec4(x_pre_up, y_pre_up, current_height, 1);

			Triple(mesh, pos_1, pos_2, pos_4);
			Triple(mesh, pos_2, pos_3, pos_4);

			num_elements += 6;
		}
	}
	mesh->SetNumElements(num_elements);
	mesh->CreateMesh();
	
}

// TODO: Fill up Geometry::GenerateCube (Slide No. 8)
void Geometry::GenerateCube(Engine::Mesh* mesh)
{
    /*   5 -- 6
     *  /|   /|
     * 1(4)-2 7
     * |    |/
     * 0 -- 3
     */
    mesh->AddAttribute(4); // for position
    mesh->AddAttribute(4); // for normal

    // Add four quad for generating a cube
    Quad(mesh, 1, 0, 3, 2);
    Quad(mesh, 2, 3, 7, 6);
    Quad(mesh, 3, 0, 4, 7);
    Quad(mesh, 6, 5, 1, 2);
    Quad(mesh, 4, 5, 6, 7);
    Quad(mesh, 5, 4, 0, 1);

    mesh->SetNumElements(36);
    mesh->CreateMesh();
    return;
}

void Geometry::GenerateTriangleMesh(Engine::Mesh* mesh)
{
    mesh->AddAttribute(4);

    glm::vec3 position_data[] = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f,1.0f, 0.0f)
    };

    for (int i = 0; i < 3; i++)
    {
        mesh->AddVertexData(position_data[i]);
        mesh->AddVertexData(1.0f);
    }
    mesh->SetNumElements(3);
    mesh->CreateMesh();
    return;
}

void Geometry::GenerateLine(Engine::Mesh* mesh)
{
    mesh->SetDrawMode(GL_LINES);
    mesh->AddAttribute(4);
    mesh->AddVertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    mesh->AddVertexData(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    mesh->SetNumElements(2);
    mesh->CreateMesh();
    return;
}

void Geometry::GenerateStar(Engine::Mesh* mesh)
{
	mesh->AddAttribute(4);
	mesh->AddAttribute(4);

	glm::vec4 position_data[] = {
		glm::vec4(1.0f * 2.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(-0.5f * 2.0f, sqrt(0.75f) * 2.0f, 0.0f, 1.0f),
		glm::vec4(-0.5f * 2.0f, -sqrt(0.75f) * 2.0f, 0.0f, 1.0f),
		glm::vec4(-1.0f * 2.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.5f * 2.0f, -sqrt(0.75f) * 2.0f, 0.0f, 1.0f),
		glm::vec4(0.5f * 2.0f, sqrt(0.75f) * 2.0f, 0.0f, 1.0f)
	};
	glm::vec4 n = glm::vec4(ComputeNormal(position_data[0], position_data[1], position_data[2]), 0.0f);
	for (int i = 0; i < 6; i++)
	{
		mesh->AddVertexData(position_data[i]);
		mesh->AddVertexData(n);
	}

	mesh->SetNumElements(6);
	mesh->CreateMesh();

	return;
}
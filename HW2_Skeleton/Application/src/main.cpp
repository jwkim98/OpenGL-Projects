// STL
#include <iostream>
#include <math.h>
#include <stdlib.h>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// include Engine
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <RenderObject.hpp>

// include application headers
#include <DefaultMaterial.hpp>
#include <PickingMaterial.hpp>
#include <LineMaterial.hpp>
#include <Geometry.hpp>
#include <Animation.hpp>
#include <picking.hpp>
#include <PickableObject.hpp>
#include <Snowman.hpp>
#include <deque>

#define _USE_MATH_DEFINES

GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;


bool move_head1 = false;
bool move_arm1 = false;
bool move_head2 = false;
bool move_arm2 = false;

// TODO: Implement gradient rectangle mesh generator for background
void GenerateGradientRectangle(Engine::Mesh* mesh)
{
	mesh->AddAttribute(4); //x,y,z,w
	mesh->AddAttribute(4); //r,g,b,w

	std::vector<glm::vec3> vect{
			glm::vec3(-10.0f, -10.0f, -1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f), //color
			glm::vec3(10.0f, -10.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f), //color
			glm::vec3(-10.0f, 10.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f), //color
			glm::vec3(-10.0f, 10.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f), //color
			glm::vec3(10.0f, -10.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f), //color
			glm::vec3(10.0f, 10.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f) //color
	};

	for (auto elem : vect) {
		mesh->AddVertexData(elem);
		mesh->AddVertexData(1.0f);
	}

	mesh->SetNumElements(6);
	mesh->CreateMesh();
	// layout 0: vec4 pos := (x,y,z,w)
	// uniform : vec4 color := (r,g,b,a)
}

void makeTriangleRecursion(int depth, glm::vec3 pointA, glm::vec3 pointB, double angle, std::deque<glm::vec3>& vertexList)
{
	if (depth == 0) return;

	if (angle >= 360) angle -= 360;
	if (angle <= -360) angle += 360;

	auto radian = static_cast<float>((angle*glm::pi<float>()) / 180);

	/// Calculate two points that divides the line in to 3
	glm::vec3 innerA = glm::vec3((2 * pointA.x + pointB.x) / 3, (2 * pointA.y + pointB.y) / 3, 0.0f);
	glm::vec3 innerB = glm::vec3((pointA.x + 2 * pointB.x) / 3, (pointA.y + 2 * pointB.y) / 3, 0.0f);

	/// Calculate length between two points, innerA and innerB
	float lengthSquared = pow((pointB.x - pointA.x), 2) + pow((pointB.y - pointA.y), 2);
	float length = sqrt(lengthSquared) / 3;

	/// Calculates length of line which is orthogonal to previous line, and ends at new point
	float lengthToNewpoint = length * (sqrt(3) / 2);

	glm::vec3 middlePoint = glm::vec3((pointA.x + pointB.x) / 2, (pointA.y + pointB.y) / 2, 0.0f);
	/// Calculate new point with given middlepoint and length to new point
	glm::vec3 newPoint = glm::vec3((middlePoint.x + lengthToNewpoint * cos(radian)),
		middlePoint.y + lengthToNewpoint * sin(radian), 0.0f);

	/// push 3 new points to vertexList vector
	vertexList.emplace_back(innerA);
	vertexList.emplace_back(newPoint);
	vertexList.emplace_back(innerB);

	/// Make recursive calls on 4 lines
	makeTriangleRecursion(depth - 1, pointA, innerA, angle, vertexList);
	makeTriangleRecursion(depth - 1, innerA, newPoint, angle - 60.0, vertexList);
	makeTriangleRecursion(depth - 1, newPoint, innerB, angle + 60.0, vertexList);
	makeTriangleRecursion(depth - 1, innerB, pointB, angle, vertexList);
}

void GenerateSnowflake(Engine::Mesh* mesh, float length, float positionX, float positionY, unsigned seed)
{
	std::deque<glm::vec3> vertexList = std::deque<glm::vec3>();// Initializes dequeue
	srand(seed);
	std::deque<glm::vec3> colorList{ glm::vec3(std::rand() / RAND_MAX ,0,0),
	glm::vec3(rand() / RAND_MAX,1,0),
	glm::vec3(rand() / RAND_MAX,0,1) };


	mesh->AddAttribute(4); //x,y,z,w
	mesh->AddAttribute(4); //r,g,b,w;

	if (vertexList.empty()) {
		auto pointA = glm::vec3(-length / 2 + positionX, -length / 2 + positionY, 0.0f);
		auto pointB = glm::vec3(length / 2 + positionX, -length / 2 + positionY, 0.0f);
		auto pointC = glm::vec3(0.0f + positionX, length*cos((30 * glm::pi<float>()) / 180) - length / 2 + positionY, 0.0f);

		vertexList.emplace_back(pointA);
		vertexList.emplace_back(pointB);
		vertexList.emplace_back(pointC);

		makeTriangleRecursion(3 - 1, pointA, pointB, -90.0, vertexList);
		makeTriangleRecursion(3- 1, pointB, pointC, 30.0, vertexList);
		makeTriangleRecursion(3 - 1, pointC, pointA, 150.0, vertexList);
	}

	if (!vertexList.empty() && !colorList.empty()) {
		size_t colorListIndex = 0;

		for (unsigned i = 0; i < vertexList.size(); i++) {
			glm::vec3 vect = vertexList.at(i);
			mesh->AddVertexData(vect); //Vertex
			mesh->AddVertexData(1.0f);
			mesh->AddVertexData(colorList.at(colorListIndex++));
			mesh->AddVertexData(1.0f);

			if (colorListIndex == colorList.size())
				colorListIndex = 0;
		}

		mesh->SetNumElements(static_cast<int>(vertexList.size()));
		mesh->CreateMesh();
	}
}

// TODO: Fill up GLFW mouse button callback function
static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
	double xpos, ypos;
	glfwGetCursorPos(a_window, &xpos, &ypos);
	xpos = xpos / ((double)g_window_width) * ((double)g_framebuffer_width);
	ypos = ypos / ((double)g_window_height) * ((double)g_framebuffer_height);

    //example code for get x position and y position of mouse click
    if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS)
    {
        int target = pick((int)xpos, (int)ypos, g_framebuffer_width, g_framebuffer_height);
        std::cout << "Picked object index: " << target << std::endl;

		switch(target)
		{
		case 1:
			move_head1 = !move_head1; break;
		case 2:
			move_arm1 = !move_arm1; break;
		case 3:
			move_head2 = !move_head2; break;
		case 4:
			move_arm2 = !move_arm2; break;
		default:
			return;
		}
    }
}

static double diff_x_left = 0;
static double diff_y_left = 0;
static double diff_x_right = 0;
static double diff_y_right = 0;

// TODO: Fill up GLFW cursor position callback function
static void CursorPosCallback(GLFWwindow* a_window, double xpos, double ypos)
{
	static double previous_xpos_left = 0, previous_ypos_left = 0;
	static double previous_xpos_right = 0, previous_ypos_right = 0;
	if (glfwGetMouseButton(a_window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_RELEASE)
	{
		if (previous_xpos_left == 0 && previous_ypos_left == 0)
		{
			previous_xpos_left = xpos;
			previous_ypos_left = ypos;
		}

		diff_x_left = xpos - previous_xpos_left;
		diff_y_left = ypos - previous_ypos_left;

		previous_xpos_left = xpos;
		previous_ypos_left = ypos;
	}
	else
	{
		previous_xpos_left = 0;
		previous_ypos_left = 0;
	}

	if (glfwGetMouseButton(a_window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_RELEASE)
	{
		if (previous_xpos_right == 0 && previous_ypos_right == 0)
		{
			previous_xpos_right = xpos;
			previous_ypos_right = ypos;
		}

		diff_x_right = xpos - previous_xpos_right;
		diff_y_right = ypos - previous_ypos_right;

		previous_xpos_right = xpos;
		previous_ypos_right = ypos;
	}
	else
	{
		previous_xpos_right = 0;
		previous_ypos_right = 0;
	}

}
static bool JumpRequest2 = false;
static bool JumpRequest1 = false;
static bool Rotate1 = false;
static bool Rotate2 = false;

static void KeyboardCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
    if (a_action == GLFW_PRESS)
    {
        switch (a_key)
        {
        case GLFW_KEY_H:
            std::cout << "CS380 Homework 2" << std::endl;
            std::cout << "keymaps:" << std::endl;
            std::cout << "h\t\t Help command" << std::endl;
            break;
		case GLFW_KEY_1:
			std::cout << "jump!" << std::endl;
			JumpRequest1 = true;
			break;
		case GLFW_KEY_2:
			std::cout << "jump2!" << std::endl;
			JumpRequest2 = true;
			break;
		case GLFW_KEY_A:
			std::cout << "rotate1" << std::endl;
			Rotate1 = !Rotate1;
			break;
		case GLFW_KEY_B:
			std::cout << "rotate2" << std::endl;
			Rotate2 = !Rotate2;
			break;
        default:
            break;
        }
    }
}


int main(int argc, char** argv)
{
    // Initialize GLFW library
    if (!glfwInit())
    {
        return -1;
    }

    // Create window and OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */
    g_window = glfwCreateWindow(g_window_width, g_window_height, "HW2 - Do you want to build a snowman?", NULL, NULL);
    if (!g_window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(g_window);

    // Initialize GLEW library
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glew_error) << std::endl;
        exit(1);
    }


    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
    glfwSetCursorPosCallback(g_window, CursorPosCallback);
    glfwSetKeyCallback(g_window, KeyboardCallback);

    // Initialize framebuffer object and picking textures
    pickingInitialize(g_framebuffer_width, g_framebuffer_width);

    Engine::Camera* main_camera = new Engine::Camera();
    main_camera->SetPosition(glm::vec3(0.0f, 0.0f, 7.5f));

    // TODO: Create mesh and material (in main.cpp) Slide No. 10 (Define cube mesh, material), No. 18 (Define pickable object)
	Geometry geometry = Geometry();



	DefaultMaterial background_material = DefaultMaterial();
	background_material.CreatePerVertexMaterial();

	Engine::Mesh background_mesh = Engine::Mesh();
	GenerateGradientRectangle(&background_mesh);

	Engine::RenderObject background = Engine::RenderObject(&background_mesh, &background_material);

	Snowman snowman1 = Snowman(1,2);
	snowman1.SetPosition(vec3(1.0, 0.0, 0.0));

	Snowman snowman2 = Snowman(3,4);
	snowman2.SetPosition(vec3(-1.0, 0.0, 0.0));


	std::deque<Engine::Mesh*> star_mesh_list;
	for( auto i =0; i < 20; i++)
	{
		Engine::Mesh* star_mesh = new Engine::Mesh();
		GenerateSnowflake(star_mesh, 5, 0, 0, i);
		star_mesh_list.emplace_back(star_mesh);
	}

	Animation animation;

	// Create star objects
	for (int i = 0; i < 20; i++)
	{
		Engine::RenderObject* star = new Engine::SnowFlake(star_mesh_list.at(i), &background_material);
		star->SetProperties(0.2,  ((rand() % 255) / 1000.0f),((rand() % 255) / 1000.0f), glm::pi<double>()/(20 + rand()%20));
		star->SetPosition(glm::vec3(-5.0f + 10.0f * ((rand() % 255) / 255.0f), 5.0f * ((rand() % 255) / 255.0f), -5.0f));
		star->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
		animation.AddObject(star);
	}

    float prev_time = 0;
	bool start1 = true;
	bool start2 = true;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        float total_time = (float)glfwGetTime();
        float elapsed_time = total_time - prev_time;
        prev_time = total_time;

		if (move_head1)
		{
			snowman1.MoveNose(100, elapsed_time);
		}
		if (move_arm1)
		{
			snowman1.MoveArm(100, elapsed_time);
		}

		if (move_head2)
		{
			snowman2.MoveNose(100, elapsed_time);
		}
		if (move_arm2)
		{
			snowman2.MoveArm(100, elapsed_time);
		}

		if(Rotate1)
		{
			snowman1.RotateHead(4, 0);
			snowman1.RotateBody(8);
		}

		if(Rotate2)
		{
			snowman2.RotateHead(-4, 0);
			snowman2.RotateBody(-8);
		}


		if(diff_x_left != 0 || diff_y_left != 0)
		{
			snowman1.Move(vec3(diff_x_left/100.0f, -diff_y_left/100.0f, 0));
			diff_x_left = 0;
			diff_y_left = 0;
		}

		if (diff_x_right != 0 || diff_y_right != 0)
		{
			snowman2.Move(vec3(diff_x_right / 100.0f, -diff_y_right / 100.0f, 0));
			diff_x_right = 0;
			diff_y_right = 0;
		}

		if(JumpRequest1)
		{
			if (start1) {
				snowman1.Jump(elapsed_time, 1, true);
				start1 = false;
			}
			else
			{
				if(!snowman1.Jump(elapsed_time, 5, false))
				{
					JumpRequest1 = false;
					start1 = true;
				}
			}
		}

		if (JumpRequest2)
		{
			if (start2) {
				snowman2.Jump(elapsed_time, 1, true);
				start2 = false;
			}
			else
			{
				if (!snowman2.Jump(elapsed_time, 5, false))
				{
					JumpRequest2 = false;
					start2 = true;
				}
			}
		}

        // First Pass: Object Selection (Slide No. 20)
        // this is for picking the object using mouse interaction
        // binding framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);
        // Background: RGB = 0x000000 => objectID: 0
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render your objects that you want to select using mouse interaction here

		snowman1.RenderPicking(main_camera);
		snowman2.RenderPicking(main_camera);
        
        // Second Pass: Object Rendering (Slide No. 11)
        // Drawing object again
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Todo: Render object with main camera in the loop

		animation.Animate(main_camera, elapsed_time);

		background.Render(main_camera);

		snowman1.Render(main_camera);
		snowman2.Render(main_camera);

        /* Swap front and back buffers */
        glfwSwapBuffers(g_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	for(auto ptr : star_mesh_list)
	{
		delete ptr;
	}

    // Delete resources
    delete main_camera;

    glfwTerminate();
    return 0;

}
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


bool move_head = false;
bool move_arm = false;

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
static double diff_x = 0;
static double diff_y = 0;
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

		if (target == 1)
		{
			move_head = !move_head;
		}
		if (target == 2)
		{
			move_arm = !move_arm;
		}
    }
}


// TODO: Fill up GLFW cursor position callback function
static void CursorPosCallback(GLFWwindow* a_window, double xpos, double ypos)
{
	static double previous_xpos = 0, previous_ypos = 0;
	if (glfwGetMouseButton(a_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		previous_xpos = 0;
		previous_ypos = 0;
		return;
	}
	if(previous_xpos == 0 && previous_ypos == 0)
	{
		previous_xpos = xpos;
		previous_ypos = ypos;
	}
	
	diff_x = xpos - previous_xpos;
	diff_y = ypos - previous_ypos;
	
	previous_xpos = xpos;
	previous_ypos = ypos;
}

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

    Engine::Mesh* cube_mesh = new Engine::Mesh();
    geometry.GenerateCube(cube_mesh);

	Animation* animation = new Animation();

    DefaultMaterial* material = new DefaultMaterial();
    material->CreateMaterial();

    PickingMaterial* picking_material = new PickingMaterial();
    picking_material->CreateMaterial();

	Engine::Mesh*  sphere_mesh1 = new Engine::Mesh();
	geometry.GenerateSphere(sphere_mesh1, 1.0);

	Engine::Mesh*  sphere_mesh2 = new Engine::Mesh();
	geometry.GenerateSphere(sphere_mesh2, 2.0);

	Engine::Mesh* cone_mesh = new Engine::Mesh();
	geometry.GenerateCone(cone_mesh, 0.5, 1.0);

	Engine::Mesh* cylinder_mesh = new Engine::Mesh();
	geometry.GenerateCylinder(cylinder_mesh, 0.3, 0.8);

    PickableObject sphere1 = PickableObject(sphere_mesh1, material);
    sphere1.SetPickingMat(picking_material);
    sphere1.SetPosition(glm::vec3(0.0f, 2.0f, 2.0f));
	sphere1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    sphere1.SetIndex(1);

    PickableObject sphere2 = PickableObject(sphere_mesh2, material);
    sphere2.SetPickingMat(picking_material);
    sphere2.SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
    sphere2.SetIndex(2);
    // TODO: Add parent (cube2) to cube1 (Slide No. 14)
    sphere2.AddParent(&sphere1);

	PickableObject cone1 = PickableObject(cone_mesh, material);
	cone1.SetPickingMat(picking_material);
	cone1.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cone1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	cone1.SetPosition(glm::vec3(-0.5f, 0.0f, 0.0f));
	cone1.SetIndex(3);
	// TODO: Add parent (cube2) to cube1 (Slide No. 14)
	cone1.AddParent(&sphere1);

	PickableObject cylinder1 = PickableObject(cylinder_mesh, material);
	cylinder1.SetPickingMat(picking_material);
	cylinder1.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cylinder1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	cylinder1.SetOrientation(glm::rotate(cylinder1.GetOrientation(), glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	cylinder1.SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
	cylinder1.SetIndex(4);
	cylinder1.AddParent(&sphere2);

	PickableObject cylinder2 = PickableObject(cylinder_mesh, material);
	cylinder2.SetPickingMat(picking_material);
	cylinder2.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cylinder2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	cylinder2.SetOrientation(glm::rotate(cylinder2.GetOrientation(), glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	cylinder2.SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	cylinder2.SetIndex(5);
	cylinder2.AddParent(&sphere2);

	DefaultMaterial background_material = DefaultMaterial();
	background_material.CreatePerVertexMaterial();

	Engine::Mesh background_mesh = Engine::Mesh();
	GenerateGradientRectangle(&background_mesh);

	Engine::RenderObject background = Engine::RenderObject(&background_mesh, &background_material);

	Snowman snowman = Snowman();
	snowman.Head = &sphere1;
	snowman.Body = &sphere2;
	snowman.RightArm = &cylinder1;
	snowman.LeftArm = &cylinder2;
	snowman.Nose = &cone1;

	std::deque<Engine::Mesh*> star_mesh_list;
	for( auto i =0; i < 20; i++)
	{
		Engine::Mesh* star_mesh = new Engine::Mesh();
		GenerateSnowflake(star_mesh, 5, 0, 0, i);
		star_mesh_list.emplace_back(star_mesh);
	}

	// Create star objects
	for (int i = 0; i < 20; i++)
	{
		Engine::RenderObject* star = new Engine::SnowFlake(star_mesh_list.at(i), &background_material);
		star->SetProperties(0.2,  ((rand() % 255) / 1000.0f),((rand() % 255) / 1000.0f), glm::pi<double>()/(20 + rand()%20));
		star->SetPosition(glm::vec3(-5.0f + 10.0f * ((rand() % 255) / 255.0f), 5.0f * ((rand() % 255) / 255.0f), 0.0f));
		star->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
		animation->AddObject(star);
	}

    float prev_time = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        float total_time = (float)glfwGetTime();
        float elapsed_time = total_time - prev_time;
        prev_time = total_time;

		if (move_head)
		{
			snowman.move_nose(100, elapsed_time);
		}
		if (move_arm)
		{
			snowman.move_arm(100, elapsed_time);
		}

		if(diff_x != 0 || diff_y != 0)
		{
			snowman.rotate_head(diff_x/3, diff_y/3);
			diff_x = 0;
			diff_y = 0;
		}
        // First Pass: Object Selection (Slide No. 20)
        // this is for picking the object using mouse interaction
        // binding framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);
        // Background: RGB = 0x000000 => objectID: 0
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render your objects that you want to select using mouse interaction here
        sphere1.RenderPicking(main_camera);
        sphere2.RenderPicking(main_camera);
        
        // Second Pass: Object Rendering (Slide No. 11)
        // Drawing object again
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Todo: Render object with main camera in the loop

        material->UpdateColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		sphere1.Render(main_camera);
        material->UpdateColor(glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
        sphere2.Render(main_camera);

		material->UpdateColor(glm::vec4(1.0f, 0.2f, 1.0f, 0.0f));
		cone1.Render(main_camera);

		cylinder1.Render(main_camera);
		cylinder2.Render(main_camera);
		
		material->UpdateColor(glm::vec4(1.0f, 215.0f/ 256.0f, 0.0f,1.0f));
		animation->Animate(main_camera, elapsed_time);

		background.Render(main_camera);

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
	delete cube_mesh;
	delete animation;
	delete material;
	delete picking_material;
	delete sphere_mesh1;
	delete sphere_mesh2;
	delete cone_mesh;
	delete cylinder_mesh;

    glfwTerminate();
    return 0;

}
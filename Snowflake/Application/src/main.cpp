// STL
#include <iostream>

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
#include <Fractal.hpp>
#include <DefaultMaterial.hpp>
#include <PerVertexColorMaterial.hpp>
#include <Snowflake.hpp>
#include <Animation.hpp>

void generatePerVertexTriangleMesh(Engine::Mesh* mesh)
{
    mesh->AddAttribute(4); //x,y,z,w
    mesh->AddAttribute(4); //r,g,b,w

    std::vector<glm::vec3> vect{
            glm::vec3(-1.0f, -1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f), //color
            glm::vec3(1.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f), //color
            glm::vec3(0.0f, 1.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f) //color
    };

    for (auto elem : vect) {
        mesh->AddVertexData(elem);
        mesh->AddVertexData(1.0f);
    }

    mesh->SetNumElements(3);
    mesh->CreateMesh();
    // layout 0: vec4 pos := (x,y,z,w)
    // uniform : vec4 color := (r,g,b,a)
}

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

// TODO: Implement custom type mesh generator for background object
void GenerateBackgroundMeshType1(Engine::Mesh* mesh)
{
    mesh->AddAttribute(4); //x,y,z,w
    mesh->AddAttribute(4); //r,g,b,w
    std::vector<glm::vec3> vertices
            {
                    glm::vec3(-5.0f, -5.0f, -0.5f),
                    glm::vec3(-1.0f, -5.0f, -0.5f),
                    glm::vec3(-5.0f, 0.0f, -0.5f),
                    glm::vec3(-5.0f, 0.0f, -0.5f),
                    glm::vec3(-1.0f, -5.0f, -0.5f),
                    glm::vec3(-1.0f, 0.0f, -0.5f)
            };

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.5f);

    for (auto vertex : vertices) {
        mesh->AddVertexData(vertex);
        mesh->AddVertexData(1.0f);
        mesh->AddVertexData(color);
        mesh->AddVertexData(1.0f);
    }

    mesh->SetNumElements(6);
    mesh->CreateMesh();
}

// TODO: Implement custom type mesh generator for background object
void GenerateBackgroundMeshType2(Engine::Mesh* mesh)
{
    mesh->AddAttribute(4); //x,y,z,w
    mesh->AddAttribute(4); //r,g,b,w

    std::vector<glm::vec3> vertices
            {
                    glm::vec3(-6.0f, 0.0f, -0.5f),
                    glm::vec3(-5.0f, 0.0f, -0.5f),
                    glm::vec3(-5.0f, 1.5f, -0.5f),

                    glm::vec3(0.0f, 0.0f, -0.5f),
                    glm::vec3(-1.0f, 1.5f, -0.5f),
                    glm::vec3(-1.0f, 0.0f, -0.5f),

                    glm::vec3(-5.0f, 1.5f, -0.5f),
                    glm::vec3(-5.0f, 0.0f, -0.5f),
                    glm::vec3(-1.0f, 0.0f, -0.5f),

                    glm::vec3(-5.0f, 1.5f, -0.5f),
                    glm::vec3(-1.0f, 0.0f, -0.5f),
                    glm::vec3(-1.0f, 1.5f, -0.5f)
            };

    glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.5f);

    for (auto vertex : vertices) {
        mesh->AddVertexData(vertex);
        mesh->AddVertexData(1.0f);
        mesh->AddVertexData(color);
        mesh->AddVertexData(1.0f);
    }

    mesh->SetNumElements(12);
    mesh->CreateMesh();
}

//void GenerateBackgroundMeshType3()

static bool Mode = true;

//key callback function which is called when key is pressed
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key==GLFW_KEY_A && action==GLFW_PRESS) {
        Mode = !Mode;
    }

}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    GLFWwindow* window;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    int width = 1024*2;
    int height = 768*2;
    window = glfwCreateWindow(width, height, "Homework 1 - Snowflake Animation 20170149", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    glViewport(0, 0, width, height);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum glewError = glewInit();
    if (glewError!=GLEW_OK) {
        std::cout << "GLEW Error: " << glewGetErrorString(glewError) << std::endl;
        exit(1);
    }

    std::cout << "Generating" << std::endl;
    Fractal fractal = Fractal(5);
    Animation animation = Animation();

    Engine::Camera mainCamera = Engine::Camera();

    std::vector<Engine::Mesh> snowflakeMeshList = std::vector<Engine::Mesh>(3000, Engine::Mesh());

    PerVertexColorMaterial material = PerVertexColorMaterial();
    material.CreateMaterial();

    fractal.addColor(glm::vec3(1, 0, 0));
    fractal.addColor(glm::vec3(0, 1, 0));
    fractal.addColor(glm::vec3(0, 0, 1));

    std::vector<Snowflake> snowflakeObjects;
    snowflakeObjects.reserve(10);


    // Set random seed with time
    time_t seed = std::time(nullptr);
    for (auto& snowflake : snowflakeMeshList) {

        /**
         * Create list of random values
         */
        float random_length = static_cast<float>(std::rand())/RAND_MAX;
        float random_positionX = static_cast<float>(std::rand())/RAND_MAX;
        float randomColorA = static_cast<float>(std::rand())/RAND_MAX;
        float randomColorB = static_cast<float>(std::rand())/RAND_MAX;
        float randomColorC = static_cast<float>(std::rand())/RAND_MAX;
        float random_speedY = static_cast<float>(std::rand())/RAND_MAX;


        fractal.resetColorList();
        fractal.addColor(glm::vec3(randomColorA, 0, 0));
        fractal.addColor(glm::vec3(0, randomColorB, 0));
        fractal.addColor(glm::vec3(0, 0, randomColorC));
        fractal.GenerateSnowflake(&snowflake, random_length, 0, 0);

        Snowflake thisSnowflake = Snowflake(&snowflake, &material, 0, -random_speedY*20, random_positionX*10-5.0f, 5);
        snowflakeObjects.emplace_back(thisSnowflake);
    }

    /// Add snowflake to animation
    for (auto& snowflakeObject : snowflakeObjects) {
        animation.AddSnowflake(&snowflakeObject);
    }

    /// Create snowflake instances
    Engine::Mesh snowflakeMesh = Engine::Mesh();

    /// Large snowflake in the middle
    fractal.resetColorList();
    fractal.addColor(glm::vec3(1.0, 0.5, 0.5));
    fractal.GenerateSnowflake(&snowflakeMesh, 3.0, 0, 0);
    Snowflake kingVirus(&snowflakeMesh, &material, 0, 0, 0, 0);


    /**
     * Generate Rectangle mesh
     */
    Engine::Mesh rectangleMesh = Engine::Mesh();
    PerVertexColorMaterial rectangleMaterial = PerVertexColorMaterial();
    rectangleMaterial.CreateMaterial();

    GenerateGradientRectangle(&rectangleMesh);
    Engine::RenderObject rectangleObject = Engine::RenderObject(&rectangleMesh, &rectangleMaterial);

    /**
     * Generate house Mesh
     */
    Engine::Mesh houseMesh = Engine::Mesh();
    PerVertexColorMaterial houseMaterial = PerVertexColorMaterial();
    houseMaterial.CreateMaterial();

    GenerateBackgroundMeshType1(&houseMesh);
    Engine::RenderObject houseObject = Engine::RenderObject(&houseMesh, &houseMaterial);

    /**
     * Generate roof Mesh
     */
    Engine::Mesh roofMesh = Engine::Mesh();
    PerVertexColorMaterial roofMaterial = PerVertexColorMaterial();
    roofMaterial.CreateMaterial();

    GenerateBackgroundMeshType2(&roofMesh);
    Engine::RenderObject roofObject = Engine::RenderObject(&roofMesh, &roofMaterial);

    mainCamera.SetPosition(glm::vec3(0.0f, 0.0f, 7.5f));

    double previousTime = 0;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        double elapsedTime = glfwGetTime();

        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// Apply constant animation (1.0f is default value. (1.0s) you should change this value for supporting consistent animation)
        animation.Mode = Mode;
        ///Animate method will move snowflake elements
        animation.Animate(static_cast<float>(elapsedTime));
        animation.Render(mainCamera);

        rectangleObject.Render(&mainCamera);
        houseObject.Render(&mainCamera);
        roofObject.Render(&mainCamera);

        ///Rotate the Kingvirus snowflake
        kingVirus.SetPosition(glm::vec3(0, 0, 0));
        kingVirus.SetOrientation(glm::rotate(kingVirus.GetOrientation(), glm::radians(1.0f), glm::vec3(0, 0, 1)));
        kingVirus.Render(&mainCamera);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}
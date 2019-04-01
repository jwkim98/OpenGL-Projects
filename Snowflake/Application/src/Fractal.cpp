#include <Fractal.hpp>
#include <cmath>

/**
 * Implement Fractal class to generate Koch snowflakes mesh (generate snowflake mesh using triangle list).
 * Koch snowflake: https://en.wikipedia.org/wiki/Koch_snowflake
 */



Fractal::Fractal(int recursionDepth)
        :_recursionDepth(recursionDepth)
{
}

void Fractal::resetColorList()
{
    _colorList = std::deque<glm::vec3>();
}

/*
 * TODO: Implement koch snowflake mesh generator
 */
void Fractal::GenerateSnowflake(Engine::Mesh* mesh, float length, float positionX, float positionY)
{
    _vertexList = std::deque<glm::vec3>();// Initializes dequeue

    mesh->AddAttribute(4); //x,y,z,w
    mesh->AddAttribute(4); //r,g,b,w
    _mesh = mesh;

    if (_vertexList.empty()) {
        auto pointA = glm::vec3(-length/2+positionX, -length/2+positionY, 0.0f);
        auto pointB = glm::vec3(length/2+positionX, -length/2+positionY, 0.0f);
        auto pointC = glm::vec3(0.0f+positionX, length*cos((30*PI)/180)-length/2+positionY, 0.0f);

        _vertexList.emplace_back(pointA);
        _vertexList.emplace_back(pointB);
        _vertexList.emplace_back(pointC);

        makeTriangleRecursion(_recursionDepth-1, pointA, pointB, -90.0);
        makeTriangleRecursion(_recursionDepth-1, pointB, pointC, 30.0);
        makeTriangleRecursion(_recursionDepth-1, pointC, pointA, 150.0);
    }

    if (!_vertexList.empty() && !_colorList.empty()) {
        size_t colorListIndex = 0;

        for (unsigned i = 0; i<_vertexList.size(); i++) {
            glm::vec3 vect = _vertexList.at(i);
            mesh->AddVertexData(vect); //Vertex
            mesh->AddVertexData(1.0f);
            mesh->AddVertexData(_colorList.at(colorListIndex++));
            mesh->AddVertexData(1.0f);

            if (colorListIndex==_colorList.size())
                colorListIndex = 0;
        }

        mesh->SetNumElements(static_cast<int>(_vertexList.size()));
        mesh->CreateMesh();
    }
}

void Fractal::addColor(glm::vec3 color)
{
    _colorList.push_back(color);
}

void Fractal::makeFractal()
{

    auto pointA = glm::vec3(-1.0f, -1.0f, 0.0f);
    auto pointB = glm::vec3(1.0f, -1.0f, 0.0f);
    auto pointC = glm::vec3(0.0f, 2*cos((30*PI)/180)-1.0f, 0.0f);

    _vertexList.emplace_back(pointA);
    _vertexList.emplace_back(pointB);
    _vertexList.emplace_back(pointC);

    float length = 2.0f;

    makeTriangleRecursion(_recursionDepth-1, pointA, pointB, -90.0);
    makeTriangleRecursion(_recursionDepth-1, pointB, pointC, 30.0);
    makeTriangleRecursion(_recursionDepth-1, pointC, pointA, 150.0);
}

void Fractal::makeTriangleRecursion(int depth, glm::vec3 pointA, glm::vec3 pointB, double angle)
{
    if (depth==0) return;

    if (angle>=360) angle -= 360;
    if (angle<=-360) angle += 360;

    auto radian = static_cast<float>((angle*PI)/180);

    /// Calculate two points that divides the line in to 3
    glm::vec3 innerA = glm::vec3((2*pointA.x+pointB.x)/3, (2*pointA.y+pointB.y)/3, 0.0f);
    glm::vec3 innerB = glm::vec3((pointA.x+2*pointB.x)/3, (pointA.y+2*pointB.y)/3, 0.0f);

    /// Calculate length between two points, innerA and innerB
    float lengthSquared = pow((pointB.x-pointA.x), 2)+pow((pointB.y-pointA.y), 2);
    float length = sqrt(lengthSquared)/3;

    /// Calculates length of line which is orthogonal to previous line, and ends at new point
    float lengthToNewpoint = length*(sqrt(3)/2);

    glm::vec3 middlePoint = glm::vec3((pointA.x+pointB.x)/2, (pointA.y+pointB.y)/2, 0.0f);
    /// Calculate new point with given middlepoint and length to new point
    glm::vec3 newPoint = glm::vec3((middlePoint.x+lengthToNewpoint*cos(radian)),
            middlePoint.y+lengthToNewpoint*sin(radian), 0.0f);

    /// push 3 new points to vertexList vector
    _vertexList.emplace_back(innerA);
    _vertexList.emplace_back(newPoint);
    _vertexList.emplace_back(innerB);

    /// Make recursive calls on 4 lines
    makeTriangleRecursion(depth-1, pointA, innerA, angle);
    makeTriangleRecursion(depth-1, innerA, newPoint, angle-60.0);
    makeTriangleRecursion(depth-1, newPoint, innerB, angle+60.0);
    makeTriangleRecursion(depth-1, innerB, pointB, angle);
}
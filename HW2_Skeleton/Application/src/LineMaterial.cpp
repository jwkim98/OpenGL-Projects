#ifdef __GNUC__
#define LINUX
#else
#define WINDOWS
#endif
#include <LineMaterial.hpp>

void LineMaterial::CreateMaterial()
{
#ifdef LINUX
    _program->AttachShader("Resources/LineMaterial/LineVertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/LineMaterial/LineFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif
#ifdef WINDOWS
    _program->AttachShader("Resources\\LineMaterial\\LineVertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources\\LineMaterial\\LineFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif

    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "color");
    glUniform4fv(location, 1, (float*)&color);
}
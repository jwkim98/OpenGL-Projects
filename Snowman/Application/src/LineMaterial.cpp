#include <LineMaterial.hpp>

void LineMaterial::CreateMaterial()
{
#ifdef WINDOWS
    _program->AttachShader("Resources\\LineMaterial\\LineVertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources\\LineMaterial\\LineFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif
#ifdef __GNUC__
    _program->AttachShader("Resources/LineMaterial/LineVertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/LineMaterial/LineFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif

    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "color");
    glUniform4fv(location, 1, (float*)&color);
}
#ifdef __GNUC__
#define LINUX
#else
#define WINDOWS
#endif

#include <PerVertexColorMaterial.hpp>

void PerVertexColorMaterial::CreateMaterial()
{
#ifdef LINUX
    _program->AttachShader("Resources/PerVertexColorMaterial/VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/PerVertexColorMaterial/FragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif

#ifdef WINDOWS
    _program->AttachShader("Resources\\PerVertexColorMaterial\\VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources\\PerVertexColorMaterial\\FragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif

	glUseProgram(_program->GetProgramId());
	GLint location = glGetUniformLocation(_program->GetProgramId(), "color");
	auto* temp = new glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4fv(location, 1, (float*)temp);
	delete temp;

}
#include "inpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Infinit {

	std::shared_ptr<Shader> Shader::Create(const string& vertexSource, const string& fragmentSource)
	{
		return std::make_shared<OpenGLShader>(OpenGLShader(vertexSource, fragmentSource));

	}

}
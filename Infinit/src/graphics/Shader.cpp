#include "inpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Infinit {

	Shader* Shader::Create(const string& vertexSource, const string& fragmentSource)
	{
		return new OpenGLShader(vertexSource, fragmentSource);

	}

}
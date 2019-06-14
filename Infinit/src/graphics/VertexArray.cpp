#include "inpch.h"
#include "VertexArray.h"

#include <Platform/OpenGL/OpenGLVertexArray.h>

namespace Infinit {

	VertexArray* VertexArray::Create()
	{
		return new OpenGLVertexArray();
	}

}
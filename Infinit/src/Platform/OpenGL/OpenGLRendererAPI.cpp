#include "inpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Infinit {

	OpenGLRendererAPI::OpenGLRendererAPI()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndexed(uint count) const
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);
	}

}
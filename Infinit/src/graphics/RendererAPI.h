#pragma once

#include "Core/Core.h"
#include <glm/glm.hpp>

namespace Infinit {

	class RendererAPI
	{
	public:
		enum class Type 
		{
			None = 0,
			OpenGL = 1
		};

		virtual void Clear() const = 0;
		virtual void SetClearColor(const glm::vec4& color) const = 0;
		virtual void DrawIndexed(uint count) const = 0;

		static void Init();
	public:
		static Type Renderer;
		static RendererAPI* s_Instance;
	};

}
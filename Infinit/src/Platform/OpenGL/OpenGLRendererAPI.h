#pragma once

#include "graphics/RendererAPI.h"

namespace Infinit {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Clear() const override;
		virtual void SetClearColor(const glm::vec4& color) const override;
		virtual void DrawIndexed(uint count) const override;
	};

}
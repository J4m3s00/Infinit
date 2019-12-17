#pragma once

namespace Infinit {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();

		virtual void Clear() const override;
		virtual void SetClearColor(const glm::vec4& color) const override;
		virtual void DrawIndexed(uint count) const override;
	};

}
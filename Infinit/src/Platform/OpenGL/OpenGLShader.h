#pragma once

#include "graphics/Shader.h"

namespace Infinit {

	class INFINIT_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const string& vertexSource, const string& fragmentSource);

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint m_RendererID;
	};

}
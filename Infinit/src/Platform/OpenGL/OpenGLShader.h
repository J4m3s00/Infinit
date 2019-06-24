#pragma once

#include "graphics/Shader.h"

namespace Infinit {

	class INFINIT_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const string& vertexSource, const string& fragmentSource);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniform1f(const string& name, const float& value) override;
		virtual void SetUniform2f(const string& name, const glm::vec2& value) override;
		virtual void SetUniform3f(const string& name, const glm::vec3& value) override;
		virtual void SetUniform4f(const string& name, const glm::vec4& value) override;
		virtual void SetUniformMat4(const string& name, const glm::mat4& value) override;
	private:
		int GetUniformLocation(const string& name);
	private:
		std::unordered_map<string, int> m_UniformBuffer;
		uint m_RendererID;
	};

}
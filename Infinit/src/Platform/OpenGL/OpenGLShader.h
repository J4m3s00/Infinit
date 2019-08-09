#pragma once

#include "graphics/Shader.h"
#include "graphics/ShaderUniforms.h"

namespace Infinit {

	

	class INFINIT_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const string& path);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniform1i(const string& name, const int& value) override;
		virtual void SetUniform1f(const string& name, const float& value) override;
		virtual void SetUniform2f(const string& name, const glm::vec2& value) override;
		virtual void SetUniform3f(const string& name, const glm::vec3& value) override;
		virtual void SetUniform4f(const string& name, const glm::vec4& value) override;
		virtual void SetUniformMat4(const string& name, const glm::mat4& value) override;

		virtual int GetResourceSlot(const string& name) const override;

		virtual bool Reload(const string& filePath) override;
	private:
		int GetUniformLocation(const string& name);
		void LoadShaderFromFile(const string& path);
		void ProcessResources();
		void ParseUniform(const string& statement);
		void ParseUniformStruct(const string& statement);
		uint ShaderTypeFromString(const string& type);
		void CompileShader();
		bool FindStruct(const string& name, ShaderStruct* struc);
	private:
		std::unordered_map<string, int> m_UniformBuffer;
		std::vector<string> m_Resources;
		std::vector<UniformMap<string, ShaderUniform*, int>> m_UniformMap;
		std::vector<ShaderStruct> m_Structs;
		string m_ShaderSource;
		uint m_RendererID;
	};

}

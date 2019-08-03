#pragma once

#include "Core/Core.h"
#include "Core/Resource.h"
#include <glm/glm.hpp>

namespace Infinit {
	
	class Shader : public Resource
	{
	public:
		Shader(const string& filePath, const string& name = "Unknown") : Resource(filePath, name) {}

		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniform1i(const string& name, const int& value) = 0;
		virtual void SetUniform1f(const string& name, const float& value) = 0;
		virtual void SetUniform2f(const string& name, const glm::vec2& value) = 0;
		virtual void SetUniform3f(const string& name, const glm::vec3& value) = 0;
		virtual void SetUniform4f(const string& name, const glm::vec4& value) = 0;
		virtual void SetUniformMat4(const string& name, const glm::mat4& value) = 0;

		virtual int GetResourceSlot(const string& name) const = 0;

		virtual bool Reload(const string& filePath) = 0;

		virtual const string& GetFilePath() const = 0;

		static std::shared_ptr<Shader> Create(const string& filePath);
	};

}
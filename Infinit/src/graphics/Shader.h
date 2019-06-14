#pragma once

#include "Core/Core.h"

namespace Infinit {
	
	class INFINIT_API Shader
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const string& vertexSource, const string& fragmentSource);
	};

}
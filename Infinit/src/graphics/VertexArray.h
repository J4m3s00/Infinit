#pragma once

#include "Core/Core.h"

namespace Infinit {

	class INFINIT_API VertexArray
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexArray* Create();
	};

}
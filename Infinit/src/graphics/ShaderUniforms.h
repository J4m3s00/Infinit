#pragma once

#include "Core/Core.h"

namespace Infinit {

	enum class ShaderDataType;

	template <typename T1, typename T2, typename T3>
	struct UniformMap
	{
		T1 first;
		T2 second;
		T3 third;
		UniformMap(T1 f, T2 s, T3 t)
			: first(f), second(s), third(t)
		{}
	};

	struct ShaderUniform
	{
		void* Data;
		ShaderUniform(uint sizeInByte) { Data = new byte[sizeInByte]; }
	};

	struct StructMember
	{
		ShaderDataType Type;
		string Name;
	};

	struct ShaderStruct
	{
		string Name;
		std::vector<StructMember> Members;
		uint Size;
	};

}
#pragma once

#include "inpch.h"
#include "Core/Core.h"
#include <glm/glm.hpp>

namespace Infinit {

	struct Light
	{
		glm::vec3 Direction;
		glm::vec3 Radiance;
	};

	typedef std::vector<Light> LightMap;

}
#pragma once

namespace Infinit {

	struct Light
	{
		glm::vec3 Direction;
		glm::vec3 Radiance;
	};

	typedef std::vector<Light> LightMap;

}
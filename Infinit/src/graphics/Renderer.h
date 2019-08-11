#pragma once

#include "Core/Core.h"
#include "Mesh.h"
//TEMP
#include "Shader.h"
#include "Camera.h"
#include "Core/Light.h"

namespace Infinit {

	class Renderer
	{
	public:
		static void Init();

		static void Begin(Camera* camera, const LightMap& lights);
		static void Draw(MeshInstance* mesh, const glm::mat4& modelMatrix = glm::mat4(1.0f));
		static void End();
	private:
		static Renderer* s_Instance;
		//TEMP
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_CameraPosition;
		LightMap m_LightMap;
	};

}
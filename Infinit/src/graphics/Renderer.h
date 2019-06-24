#pragma once

#include "Core/Core.h"
#include "Mesh.h"
//TEMP
#include "Shader.h"
#include "Camera.h"

namespace Infinit {

	class Renderer
	{
	public:
		static void Init();

		static void Begin(std::shared_ptr<Camera> camera);
		static void Draw(Mesh* mesh);
		static void End();
	private:
		static Renderer* s_Instance;
		//TEMP
		static std::unique_ptr<Shader> m_Shader;
	};

}
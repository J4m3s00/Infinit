#include "inpch.h"
#include "Renderer.h"

#include "RendererAPI.h"

namespace Infinit {

	Renderer* Renderer::s_Instance = nullptr;

	void Renderer::Init()
	{
		s_Instance = new Renderer();
	}

	template<typename T>
	bool isEqual(std::vector<T> const &v1, std::vector<T> const &v2)
	{
		return (v1.size() == v2.size() &&
			std::equal(v1.begin(), v1.end(), v2.begin()));
	}


	void Renderer::Begin(Camera* camera, const LightMap& lightMap)
	{
		IN_CORE_ASSERT(camera, "Set a Camera to Render properly");
		RendererAPI::s_Instance->Clear();
		s_Instance->m_ViewProjectionMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();
		s_Instance->m_CameraPosition = camera->GetPosition();
		//Make that better
		s_Instance->m_LightMap.clear();
		s_Instance->m_LightMap = lightMap;
	}

	void Renderer::End()
	{
	}

	void Renderer::Draw(MeshInstance* mesh, const glm::mat4& modelMatrix)
	{
		IN_CORE_ASSERT(s_Instance, "No Renderer instance set!"); //Forgot to call Renderer::Init(); ?
		IN_CORE_ASSERT(mesh, "Mesh not valid");
		IN_CORE_ASSERT(s_Instance->m_LightMap.size() > 0, "No lights set for the scene!");
		//IN_CORE_ASSERT(mesh->UsedMaterial, "Pls provide a Material for the model!");
		if (!mesh->UsedMaterial)
		{
			static bool warningShows = false;
			if (!warningShows)
				IN_CORE_WARN("Pls provide a Material fdor the model!");
			warningShows = true;
			return;
		}

		//Clean this up!

		std::weak_ptr<Shader> shader = mesh->UsedMaterial->ShaderProgram;
		if (shader.expired()) return;
		byte* uniformBuffer = shader.lock()->GetUniformBuffer("u_ViewProjectionMatrix");
		memcpy(uniformBuffer, &s_Instance->m_ViewProjectionMatrix[0][0], sizeof(float) * 4 * 4);
		//*((glm::mat4*)(uniformBuffer)) = s_Instance->m_ViewProjectionMatrix;
		//shader.lock()->SetUniformMat4("u_ViewProjectionMatrix", s_Instance->m_ViewProjectionMatrix);
		uniformBuffer = shader.lock()->GetUniformBuffer("u_ModelMatrix");
		memcpy(uniformBuffer, &modelMatrix[0][0], sizeof(float) * 4 * 4);
		//((glm::mat4&)(*uniformBuffer)) = modelMatrix;
		//shader.lock()->SetUniformMat4("u_ModelMatrix", modelMatrix);

		uniformBuffer = shader.lock()->GetUniformBuffer("lights");
		memcpy(uniformBuffer, &s_Instance->m_LightMap[0], sizeof(Light));
		shader.lock()->SetUniform3f("lights.Direction", s_Instance->m_LightMap[0].Direction * TO_RADIANS);
		//shader.lock()->SetUniform3f("lights.Radiance", s_Instance->m_LightMap[0].Radiance);
		
		uniformBuffer = shader.lock()->GetUniformBuffer("u_CameraPosition");
		memcpy(uniformBuffer, &s_Instance->m_CameraPosition, sizeof(glm::vec3));
		//shader.lock()->SetUniform3f("u_CameraPosition", s_Instance->m_CameraPosition);

		mesh->UsedMaterial->Bind();

		mesh->GetVertexArray()->Bind();
		RendererAPI::s_Instance->DrawIndexed(mesh->GetVertexCount());
	}

}
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
		IN_CORE_ASSERT(mesh->Material, "Pls provide a Material for the model!");

		mesh->Material->Bind();
		std::weak_ptr<Shader> shader = mesh->Material->ShaderProgram;
		shader.lock()->SetUniformMat4("u_ViewProjectionMatrix", s_Instance->m_ViewProjectionMatrix);
		shader.lock()->SetUniformMat4("u_ModelMatrix", modelMatrix);
		shader.lock()->SetUniform3f("lights.Direction", s_Instance->m_LightMap[0].Direction * TO_RADIANS);
		shader.lock()->SetUniform3f("lights.Radiance", s_Instance->m_LightMap[0].Radiance);
		shader.lock()->SetUniform3f("u_CameraPosition", s_Instance->m_CameraPosition);

		mesh->GetVertexArray()->Bind();
		RendererAPI::s_Instance->DrawIndexed(mesh->GetVertexCount());
	}

}
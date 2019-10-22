#include "inpch.h"
#include "Renderer.h"

#include "RendererAPI.h"

namespace Infinit {

	Renderer* Renderer::s_Instance = nullptr;

	void Renderer::Init()
	{
		s_Instance = new Renderer();
	}

	void Renderer::Begin(Camera* camera, const LightMap& lightMap)
	{
		IN_CORE_ASSERT(camera, "Set a Camera to Render properly");
		IN_RENDER({

		Infinit::RendererAPI::s_Instance->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RendererAPI::s_Instance->Clear();
			})
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

		uniformBuffer = shader.lock()->GetUniformBuffer("u_ModelMatrix");
		memcpy(uniformBuffer, &modelMatrix[0][0], sizeof(float) * 4 * 4);

		uniformBuffer = shader.lock()->GetUniformBuffer("lights");
		memcpy(uniformBuffer, &s_Instance->m_LightMap[0], sizeof(Light));
		shader.lock()->SetUniform3f("lights.Direction", s_Instance->m_LightMap[0].Direction * TO_RADIANS);
		
		uniformBuffer = shader.lock()->GetUniformBuffer("u_CameraPosition");
		memcpy(uniformBuffer, &s_Instance->m_CameraPosition, sizeof(glm::vec3));

		mesh->UsedMaterial->Bind();

		mesh->GetVertexArray()->Bind();
		uint vertexCount = mesh->GetVertexCount();
		IN_RENDER1(vertexCount, {
				RendererAPI::s_Instance->DrawIndexed(vertexCount);
			})
	}

	void Renderer::WaitAndRender()
	{
		m_CommandQueue.Execute();
	}

}
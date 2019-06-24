#include "inpch.h"
#include "Renderer.h"

#include "RendererAPI.h"

namespace Infinit {

	Renderer* Renderer::s_Instance = nullptr;
	std::unique_ptr<Shader> Renderer::m_Shader;

	void Renderer::Init()
	{
		s_Instance = new Renderer();
		m_Shader.reset(Shader::Create(
			R"(
			#version 330 core
			
			layout (location = 0) in vec3 position;
			layout (location = 1) in vec3 normal;
			layout (location = 2) in vec2 texCoords;
			layout (location = 3) in vec3 tangent;
			layout (location = 4) in vec3 binormal;

			out vec4 vColor;

			uniform mat4 pr_matrix;
			uniform mat4 vw_matrix;

			void main()
			{
				vColor = vec4(0.3f, 0.4f, 0.8f, 1.0f);
				gl_Position = pr_matrix * vw_matrix * vec4(position, 1.0);
			}
			)",

			R"(
			#version 330 core

			layout (location = 0) out vec4 fragColor;

			in vec4 vColor;

			void main()
			{
				fragColor = vColor;
			}
)"
));
	}

	void Renderer::Begin(std::shared_ptr<Camera> camera)
	{
		RendererAPI::s_Instance->Clear();
		m_Shader->Bind();
		m_Shader->SetUniformMat4("pr_matrix", camera->GetProjectionMatrix());
		m_Shader->SetUniformMat4("vw_matrix", camera->GetViewMatrix());
	}

	void Renderer::End()
	{
		m_Shader->Unbind();
	}

	void Renderer::Draw(Mesh* mesh)
	{
		IN_CORE_ASSERT(mesh, "Mesh not valid");
		mesh->GetVertexArray()->Bind();

		RendererAPI::s_Instance->DrawIndexed(mesh->GetVertexArray()->GetIndexBuffer()->GetCount());
	}

}
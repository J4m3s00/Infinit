#include <inpch.h>
#include "Application.h"
#include "Log.h"
#include "Layer/Layer.h"

#include <glad/glad.h>

#include <graphics/Shader.h>
#include <graphics/VertexArray.h>
#include <graphics/Buffer.h>

#include <glm/glm.hpp>

namespace Infinit {

	struct Vertex
	{
		glm::vec3 position;
		uint color;
	};

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name) 
		: m_Name(name), m_LayerStack()
	{
		s_Instance = this;
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* layer)
	{
		m_LayerStack.PopOverlay(layer);
		layer->OnDetach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher disp(e);
		disp.Dispatch<WindowCloseEvent>(IN_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		m_Running = Init();
		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			//TEST RENDER
			m_Shader->Bind();
			m_VAO->Bind();

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

			m_VAO->Unbind();
			m_Shader->Unbind();

			ImGuiLayer::Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			ImGuiLayer::End();

			m_Window->Update();
		}
	}

	bool Application::Init()
	{
		m_Window.reset(new Window({ m_Name, 1270, 720 }));
		if (!m_Window) return false;
		m_Window->SetEventCallback(IN_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushLayer(m_ImGuiLayer);



		m_Shader = Shader::Create(
			R"(
			#version 330 core
			
			layout (location = 0) in vec3 position;
			layout (location = 1) in vec4 color;

			out vec4 vColor;

			void main()
			{
				vColor = color;
				gl_Position = vec4(position, 1.0);
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
);

		Vertex vertices[] = {
			{{-0.5f, -0.5f, 0.0f}, 0xff0000ff },
			{{ 0.0f,  0.5f, 0.0f}, 0xff00ffff},
			{{ 0.5f, -0.5f, 0.0f}, 0xffff00ff}
		};

		uint indices[] = {
			0, 1, 2
		};

		m_VAO.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, 3 * sizeof(Vertex)));
		
		vertexBuffer->SetLayout({ {ShaderDataType::Float3, "position"},
									{ShaderDataType::Byte4, "color", true} });
		m_VAO->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, 3));
		m_VAO->SetIndexBuffer(indexBuffer);

		return true;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
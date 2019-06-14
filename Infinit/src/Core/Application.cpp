#include <inpch.h>
#include "Application.h"
#include "Log.h"
#include "Layer/Layer.h"

#include <glad/glad.h>

#include <graphics/Shader.h>
#include <graphics/VertexArray.h>
#include <graphics/Buffer.h>

namespace Infinit {

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
			m_IBO->Bind();

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

			m_IBO->Unbind();
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

			void main()
			{
				gl_Position = vec4(position, 1.0);
			}
			)",

			R"(
			#version 330 core

			layout (location = 0) out vec4 fragColor;

			void main()
			{
				fragColor = vec4(1.0, 0.0, 0.0, 1.0);
			}
)"
);
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};

		uint indices[] = {
			0, 1, 2
		};

		m_VAO = VertexArray::Create();

		m_VBO = VertexBuffer::Create(vertices, 9 * sizeof(float));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		m_IBO = IndexBuffer::Create(indices, 3);


		return true;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
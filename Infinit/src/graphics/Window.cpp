#include "inpch.h"
#include "Window.h"
#include "Core/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Events/WindowEvent.h"
#include "Events/InputEvent.h"
#include "Core/KeyCodes.h"

namespace Infinit {

	Window::Window(const WindowProp& prop)
	{
		m_Data.title = prop.title;
		m_Data.width = prop.width;
		m_Data.height = prop.height;

		glfwInit();

		m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), NULL, NULL);
		if (!m_Window)
		{
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_Window);
		int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		IN_CORE_ASSERT(gladStatus, "Failed to initialize glad!");
		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.callback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.callback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keycode, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent pressed(keycode, scancode, mods);
					data.callback(pressed);
					KeyDownEvent down(keycode, scancode, mods);
					data.callback(down);
					break;
				}
				case GLFW_REPEAT: 
				{
					KeyDownEvent down(keycode, scancode, mods);
					data.callback(down);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent rel(keycode, scancode, mods);
					data.callback(rel);
					break;
				}
			}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event(xPos, yPos);
			data.callback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button, mods);
					data.callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button, mods);
					data.callback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event(xOffset, yOffset);
			data.callback(event);
		});

		
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::Update() const
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	bool Window::IsClosed() const
	{
		return (bool)glfwWindowShouldClose(m_Window);
	}

}
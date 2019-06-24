#include "inpch.h"
#include "Input.h"

#include <GLFW/glfw3.h>
#include "Core/Application.h"

namespace Infinit {

	bool Input::IsKeyPressed(int keycode)
	{
		auto& window = Application::Get().GetWindow();
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window.GetNativeWindow()), keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMousePressed(int button)
	{
		auto& window = Application::Get().GetWindow();

		auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window.GetNativeWindow()), button);
		return state == GLFW_PRESS;
	}

	float Input::GetMouseX()
	{
		auto& window = Application::Get().GetWindow();

		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeWindow()), &xpos, &ypos);

		return (float)xpos;
	}

	float Input::GetMouseY()
	{
		auto& window = Application::Get().GetWindow();

		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeWindow()), &xpos, &ypos);

		return (float)ypos;
	}

}
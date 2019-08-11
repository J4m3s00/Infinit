#include "inpch.h"

#include <imgui.h>
#include <Core/Application.h>

#include "Core/Log.h"
#include "ImGuiLayer.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

namespace Infinit {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::ImGuiLayer(const string& name)
		: Layer(name)
	{}

	void ImGuiLayer::OnAttach()
	{
		
	}

	void ImGuiLayer::OnDetach() 
	{
		
	}

	void ImGuiLayer::Begin()
	{
		
	}

	void ImGuiLayer::End()
	{
		
	}

	void ImGuiLayer::OnImGuiRender()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}

}
#include "inpch.h"

namespace Infinit {

	void Resource::ImGuiDraw()
	{
		ImGui::Text("FilePath: %s", m_FilePath.c_str());
		ImGui::SameLine();
		ImGui::Text("Name: %s", m_Name.c_str());
		if (ImGui::Button("Save##Resource"))
		{
			Application::Get().GetResourceLoader().SaveResource(this);
		}
	}

}
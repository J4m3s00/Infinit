#include "inpch.h"

namespace Infinit {

	Resource::Resource(const string& filePath, Type resourceType, const string& name) : Object(name),
		m_FilePath("FilePath", this, filePath),
		m_ResourceType("ResourceType", this, (int)resourceType) {}

	void Resource::ImGuiDraw()
	{
		ImGui::Text("FilePath: %s", m_FilePath.GetValue().c_str());
		ImGui::SameLine();
		ImGui::Text("Name: %s", m_Name.GetValue().c_str());
		if (ImGui::Button("Save##Resource"))
		{
			Application::Get().GetResourceLoader().SaveResource(this);
		}
	}

}
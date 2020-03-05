#include "inpch.h"

namespace Infinit {

	void Resource::OnSerialize(json& js_object)
	{
		return { {"Name", m_Name}, {"FilePath", m_FilePath}, {"Type", m_ResourceType} };
	}

	void Resource::OnDeserialize(const json& js_object)
	{
		m_FilePath = json_object["FilePath"]; m_Name = json_object["Name"]; m_ResourceType = json_object["Type"];
	}

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
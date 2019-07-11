#include "inpch.h"
#include "ImGuiHelper.h"

#include "Core/Application.h"
#include "graphics/Texture.h"

namespace Infinit {

	void ImGuiProperty(const string& name, int& value, int min, int max, PropertyFlags flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragInt(id.c_str(), &value);
		else
			ImGui::SliderInt(id.c_str(), &value, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, int& value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, bool& value)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		ImGui::Checkbox(id.c_str(), &value);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, float& value, float min, float max, PropertyFlags flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragFloat(id.c_str(), &value);
		else
			ImGui::SliderFloat(id.c_str(), &value, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, float& value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, glm::vec2& value, float min, float max, PropertyFlags flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragFloat2(id.c_str(), &value[0]);
		else
			ImGui::SliderFloat2(id.c_str(), &value[0], min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, glm::vec2& value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, glm::vec3& value, float min, float max, PropertyFlags flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::ColorProperty)
			ImGui::ColorEdit3(id.c_str(), &value[0]);
		else if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragFloat3(id.c_str(), &value[0]);
		else
			ImGui::SliderFloat3(id.c_str(), &value[0], min, max);


		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, glm::vec3& value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, glm::vec4& value, float min, float max, PropertyFlags flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::ColorProperty)
			ImGui::ColorEdit4(id.c_str(), &value[0]);
		else if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragFloat4(id.c_str(), &value[0]);
		else
			ImGui::SliderFloat4(id.c_str(), &value[0], min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, glm::vec4& value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, std::shared_ptr<Texture> tex)
	{
		ImGui::Text(name.c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
		if (tex)
			ImGui::Image((void*)tex->GetRendererID(), ImVec2(64, 64));
		ImGui::PopStyleVar();
		if (ImGui::IsItemHovered())
		{
			if (tex)
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(tex->GetPath().c_str());
				ImGui::PopTextWrapPos();
				ImGui::Image((void*)tex->GetRendererID(), ImVec2(384, 384));
				ImGui::EndTooltip();
			}
			if (ImGui::IsItemClicked())
			{
				std::string filename = Application::Get().OpenFile("");
				if (filename != "")
					tex->Reload(filename);
			}
		}
	}

}
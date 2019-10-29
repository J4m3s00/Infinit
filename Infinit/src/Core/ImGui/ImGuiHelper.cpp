#include "inpch.h"
#include "ImGuiHelper.h"

#include "Core/Application.h"
#include "graphics/Texture.h"

namespace Infinit {

	void ImGuiProperty(const string& name, int* value, int min, int max, PropertyFlags flags)
	{
		if (!value) return;
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragInt(id.c_str(), value);
		else
			ImGui::SliderInt(id.c_str(), value, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, int* value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, bool* value)
	{
		if (!value) return;
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		ImGui::Checkbox(id.c_str(), value);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, float* value, float min, float max, PropertyFlags flags)
	{
		if (!value) return;
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragFloat(id.c_str(), value);
		else
			ImGui::SliderFloat(id.c_str(), value, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, float* value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, glm::vec2* value, float min, float max, PropertyFlags flags)
	{
		if (!value) return;
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragFloat2(id.c_str(), &value->x);
		else
			ImGui::SliderFloat2(id.c_str(), &value->x, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, glm::vec2* value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, glm::vec3* value, float min, float max, PropertyFlags flags)
	{
		if (!value) return;
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::ColorProperty)
			ImGui::ColorEdit3(id.c_str(), &value->x);
		else if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragFloat3(id.c_str(), &value->x);
		else
			ImGui::SliderFloat3(id.c_str(), &value->x, min, max);


		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, glm::vec3* value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, glm::vec4* value, float min, float max, PropertyFlags flags)
	{
		if (!value) return;
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::ColorProperty)
			ImGui::ColorEdit4(id.c_str(), &value->x);
		else if ((int)flags & (int)PropertyFlags::DragFloat)
			ImGui::DragFloat4(id.c_str(), &value->x);
		else
			ImGui::SliderFloat4(id.c_str(), &value->x, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, glm::vec4* value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	std::shared_ptr<Texture2D> ImGuiPropertyTex2D(const string& name, std::shared_ptr<Texture2D> tex)
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
				std::string filename = Application::Get().OpenFile(IN_FILE_FILTER_Texture);
				if (filename != "")
				{
					if (tex)
						tex->Reload(filename);
					else
						tex = Application::Get().GetResourceLoader().GetResource<Texture2D>(filename);
				}
			}
		}
		return tex;
	}

	std::shared_ptr<TextureCube> ImGuiPropertyTexCube(const string& name, std::shared_ptr<TextureCube> tex)
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
				std::string filename = Application::Get().OpenFile(IN_FILE_FILTER_Cubemaps);
				if (filename != "")
				{
					if (tex)
						tex->Reload(filename);
					else
						tex = Application::Get().GetResourceLoader().GetResource<TextureCube>(filename);
				}
			}
		}
		return tex;
	}

}
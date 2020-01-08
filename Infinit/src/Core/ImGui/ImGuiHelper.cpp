#include "inpch.h"

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

	void ImGuiProperty(const string& name, glm::mat4* value, float min, float max, PropertyFlags flags)
	{
		glm::vec4* r0 = &(*value)[0];
		glm::vec4* r1 = &(*value)[1];
		glm::vec4* r2 = &(*value)[2];
		glm::vec4* r3 = &(*value)[3];
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::DragFloat)
		{
			ImGui::DragFloat4((id + "r0").c_str(), &r0->x, min, max);
			ImGui::DragFloat4((id + "r1").c_str(), &r1->x, min, max);
			ImGui::DragFloat4((id + "r2").c_str(), &r2->x, min, max);
			ImGui::DragFloat4((id + "r3").c_str(), &r3->x, min, max);
		}
		else
		{
			ImGui::SliderFloat4((id + "r0").c_str(), &r0->x, min, max);
			ImGui::SliderFloat4((id + "r1").c_str(), &r1->x, min, max);
			ImGui::SliderFloat4((id + "r2").c_str(), &r2->x, min, max);
			ImGui::SliderFloat4((id + "r3").c_str(), &r3->x, min, max);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, glm::mat4* value, PropertyFlags flags)
	{
		ImGuiProperty(name, value, -1.0f, 1.0f, flags);
	}

	void ImGuiProperty(const string& name, glm::mat3* value, float min, float max, PropertyFlags flags)
	{
		glm::vec3* r0 = &(*value)[0];
		glm::vec3* r1 = &(*value)[1];
		glm::vec3* r2 = &(*value)[2];
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		string id = "##" + name;
		if ((int)flags & (int)PropertyFlags::DragFloat)
		{
			ImGui::DragFloat3((id + "r0").c_str(), &r0->x, min, max);
			ImGui::DragFloat3((id + "r1").c_str(), &r1->x, min, max);
			ImGui::DragFloat3((id + "r2").c_str(), &r2->x, min, max);
		}
		else
		{
			ImGui::SliderFloat3((id + "r0").c_str(), &r0->x, min, max);
			ImGui::SliderFloat3((id + "r1").c_str(), &r1->x, min, max);
			ImGui::SliderFloat3((id + "r2").c_str(), &r2->x, min, max);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ImGuiProperty(const string& name, glm::mat3* value, PropertyFlags flags)
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
		ImGui::Button("", ImVec2(64, 64));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_NODE"))
			{
				ResourceNode* node = (ResourceNode*)payload->Data;
				if (node->GetType() == Resource::Type::TEXTURE)
				{
					tex = node->GetResource<Texture2D>();
				}
			}
			ImGui::EndDragDropTarget();
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
		ImGui::Button("", ImVec2(64, 64));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_NODE"))
			{
				ResourceNode* node = (ResourceNode*)payload->Data;
				if (node->GetType() == Resource::Type::CUBEMAP)
				{
					tex = node->GetResource<TextureCube>();
				}
			}
			ImGui::EndDragDropTarget();
		}
		return tex;
	}

}
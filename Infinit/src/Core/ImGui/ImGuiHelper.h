#pragma once

#include "Core/Core.h"
#include <imgui.h>
#include <glm/glm.hpp>

namespace Infinit {

	class Texture;

	//TODO
	struct SliderSettings
	{

	};

	enum class PropertyFlags
	{
		None = 0,
		ColorProperty = BIT(1),


		DragFloat = BIT(30)
	};

	extern void ImGuiProperty(const string& name, int& value, int min = -100, int max = 100, PropertyFlags flags = PropertyFlags::None);
	extern void ImGuiProperty(const string& name, int& value, PropertyFlags flags);
	extern void ImGuiProperty(const string& name, bool& value);
	extern void ImGuiProperty(const string& name, float& value, float min = -1.0f, float max = 1.0f, PropertyFlags flags = PropertyFlags::None);
	extern void ImGuiProperty(const string& name, float& value, PropertyFlags flags);
	extern void ImGuiProperty(const string& name, glm::vec2& value, float min = -1.0f, float max = 1.0f, PropertyFlags flags = PropertyFlags::None);
	extern void ImGuiProperty(const string& name, glm::vec2& value, PropertyFlags flags);
	extern void ImGuiProperty(const string& name, glm::vec3& value, float min = -1.0f, float max = 1.0f, PropertyFlags flags = PropertyFlags::None);
	extern void ImGuiProperty(const string& name, glm::vec3& value, PropertyFlags flags);
	extern void ImGuiProperty(const string& name, glm::vec4& value, float min = -1.0f, float max = 1.0f, PropertyFlags flags = PropertyFlags::None);
	extern void ImGuiProperty(const string& name, glm::vec4& value, PropertyFlags flags);
	extern void ImGuiProperty(const string& name, std::shared_ptr<Texture> tex);

}
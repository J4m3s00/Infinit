#include "inpch.h"
#include "Component.h"

#include <imgui.h>

namespace Infinit {

	void Component::DrawImGui()
	{
		ImGui::CollapsingHeader(GetTypeName().c_str());
	}

}
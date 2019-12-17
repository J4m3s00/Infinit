#include "inpch.h"

namespace Infinit {

	void Component::DrawImGui()
	{
		ImGui::CollapsingHeader(GetTypeName().c_str());
	}

}
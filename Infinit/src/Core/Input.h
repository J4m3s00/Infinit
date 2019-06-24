#pragma once

#include "Core/Core.h"
#include "Core/KeyCodes.h"

namespace Infinit {
	
	class Input
	{
	public:
		static bool IsKeyPressed(int keyCode);
		static bool IsMousePressed(int button);

		static float GetMouseX();
		static float GetMouseY();
	};

}
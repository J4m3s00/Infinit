#pragma once

#include "Core/Core.h"
#include <string>

namespace Infinit {

	class INFINIT_API Window
	{
	public:
		Window(const string& title, uint width, uint height);
		~Window();

		void Update() const;
		bool IsClosed() const;
	private:
		uint m_Width;
		uint m_Height;
		string m_Title;
	};

}
#pragma once

#include "Core/Core.h"
#include "inpch.h"

namespace Infinit {
	class Layer;

	class INFINIT_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_LayerStack.begin(); }
		std::vector<Layer*>::iterator end() { return m_LayerStack.end(); }
	private:
		uint m_StackIndex = 0;
		std::vector<Layer*> m_LayerStack;
	};

}
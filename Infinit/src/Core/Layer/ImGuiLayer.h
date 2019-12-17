#pragma once

namespace Infinit {


	class INFINIT_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const string& name);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		static void Begin();
		static void End();
	};

}
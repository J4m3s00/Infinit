#pragma once

#include "Core/Core.h"
#include "Core/Light.h"
#include "graphics/Camera.h"
#include "Core/Layer/LayerStack.h"

namespace Infinit {

	class Scene
	{
	public:
		Scene(const string& name);
		~Scene();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		void Detach();
		void Attach();
		void Update();
		void Render();
		void ImGuiRender();

		void SetActiveCamera(Camera* activeCamera);
		void SetLightMap(LightMap lightMap) { m_LightMap = lightMap; }
	private:
		LightMap m_LightMap;
		Camera*  m_ActiveCamera;
		LayerStack m_LayerStack;
		//maybe offline cameras for effects
		//std::vector<std::shared_ptr<Camera>> m_OfflineCameras;

		string m_Name;
	};

}
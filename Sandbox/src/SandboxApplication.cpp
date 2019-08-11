#include <Infinit.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class TestLayer : public Infinit::Layer
{
private:
	std::shared_ptr<Infinit::Mesh> m_Mesh;
	std::shared_ptr<Infinit::MeshInstance> m_Instance;
	Infinit::Camera* m_Camera;
	
	std::shared_ptr<Infinit::Shader> m_Shader;

	bool m_UseAlbedo = false;
	bool m_UseMetalness = false;
	bool m_UseNormal = false;
	bool m_UseRoughness = false;
	float m_RadianceFilter = 0.0f;
	float m_EnvMapRotation = 0.0f;
	glm::vec3 m_AlbedoColor = {0.5f, 0.5f, 0.5f};
	float m_Metalness = 0.5f;
	float m_Roughness = 0.5f;
	Infinit::Light* m_Light;
public:
	TestLayer()
		: Infinit::Layer("Test Layer")
	{

	} 



	virtual void OnAttach() override
	{
		m_Camera = new Infinit::Camera(glm::perspective(65.0f, 16.0f / 9.0f, 0.0001f, 10000.0f));
		Scene->ActiveCamera = m_Camera;
		Scene->LightMap.push_back({ { 0.2f, 0.34f, 0.5f }, { 1.0f, 1.0f, 1.0f } });
		m_Light = &Scene->LightMap[0];
		m_Shader = std::dynamic_pointer_cast<Infinit::Shader>(Infinit::Application::Get().GetResource("res/shaders/pbr.shader"));
		//Infinit::Shader::Create("pbr.shader");
		//m_Mesh.reset(new Infinit::Mesh("res/cerberus.fbx"));
		m_Mesh = std::dynamic_pointer_cast<Infinit::Mesh>(Infinit::Application::Get().GetResource("res/cerberus.fbx"));

		m_Instance.reset(new Infinit::MeshInstance(m_Mesh));
		std::shared_ptr<Infinit::Material> mat = std::make_shared<Infinit::Material>(Infinit::Material(m_Shader));
		mat->AddTexture("u_AlbedoTexture", std::dynamic_pointer_cast<Infinit::Texture2D>(Infinit::Application::Get().GetResource("res/cerberus_A.png")));
		mat->AddParameter(new Infinit::MaterialParameter<bool>("u_AlbedoTexToggle"));
		mat->AddParameter(new Infinit::MaterialParameter<glm::vec3>("u_AlbedoColor"));
		mat->AddTexture("u_NormalTexture", std::dynamic_pointer_cast<Infinit::Texture2D>(Infinit::Application::Get().GetResource("res/cerberus_N.png")));
		mat->AddParameter(new Infinit::MaterialParameter<bool>("u_NormalTexToggle"));
		mat->AddTexture("u_MetalnessTexture", std::dynamic_pointer_cast<Infinit::Texture2D>(Infinit::Application::Get().GetResource("res/cerberus_M.png")));
		mat->AddParameter(new Infinit::MaterialParameter<bool>("u_MetalnessTexToggle"));
		mat->AddParameter(new Infinit::MaterialParameter<float>("u_Metalness"));
		mat->AddTexture("u_RoughnessTexture", std::dynamic_pointer_cast<Infinit::Texture2D>(Infinit::Application::Get().GetResource("res/cerberus_R.png")));
		mat->AddParameter(new Infinit::MaterialParameter<bool>("u_RoughnessTexToggle"));
		mat->AddParameter(new Infinit::MaterialParameter<float>("u_Roughness"));
		mat->AddTexture("u_EnvRadianceTex", std::dynamic_pointer_cast<Infinit::TextureCube>(Infinit::Application::Get().GetResource("res/Arches_E_PineTree_Radiance.cubemap")));
		mat->AddTexture("u_EnvIrradianceTex", std::dynamic_pointer_cast<Infinit::TextureCube>(Infinit::Application::Get().GetResource("res/Arches_E_PineTree_Irradiance.cubemap")));
		mat->AddTexture("u_BRDFLUTTexture", std::dynamic_pointer_cast<Infinit::Texture2D>(Infinit::Application::Get().GetResource("res/BRDF_LUT.tga")));
		mat->AddParameter(new Infinit::MaterialParameter<float>("u_RadiancePrefilter"));
		mat->AddParameter(new Infinit::MaterialParameter<float>("u_EnvMapRotation"));
		m_Instance->Material = mat;

		Infinit::GameObject* go = new Infinit::GameObject("Name");
		go->AddComponent<Infinit::MeshComponent>()->Instance = m_Instance.get();
		AddGameObject(go);
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate() override
	{
	}

	virtual void OnRender() override
	{
	}

	virtual void OnImGuiRender() override
	{
		
	}

	virtual void OnEvent(Infinit::Event& e) override
	{
		Layer::OnEvent(e);
		if (e.GetEventType() == Infinit::EventType::AppUpdate)
		{
			m_Camera->Update();
		}
		else if (e.GetEventType() == Infinit::EventType::AppImGuiRender)
		{
			ImGui::Begin("Light");
			ImGui::SliderFloat3("Direction", &m_Light->Direction[0], -360.0f, 360.0f);
			ImGui::SliderFloat3("Radiance", &m_Light->Radiance[0], 0.0f, 1.0f);
			ImGui::End();



			Scene->DrawImGui();
		}
		else if (e.GetEventType() == Infinit::EventType::KeyPressed)
		{
			Infinit::KeyPressedEvent& keyPress = (Infinit::KeyPressedEvent&)e;
			if (keyPress.GetKeyCode() == IN_KEY_R && keyPress.GetMods() == IN_MOD_CONTROL)
			{
				m_Shader->Reload("");
			}
		}
	}

};

class Sandbox : public Infinit::Application
{
private:
	Infinit::Scene scene;
public:
	Sandbox()
		: Application("Hello World", Infinit::RendererAPI::Type::OpenGL), scene("Test Scene")
	{
		scene.PushLayer(new TestLayer());
		SetActiveScene(&scene);
	}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}

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
		m_Shader = Infinit::Shader::Create("pbr.shader");
		m_Mesh.reset(new Infinit::Mesh("cerberus.fbx"));

		m_Instance.reset(new Infinit::MeshInstance(m_Mesh));
		std::shared_ptr<Infinit::Material> mat = std::make_shared<Infinit::Material>(Infinit::Material(m_Shader));
		mat->AddTexture("u_AlbedoTexture", Infinit::Texture2D::Create(Infinit::TextureFormat::RGB, 1, 1));
		mat->AddParameter(new Infinit::MaterialParameter("u_AlbedoTexToggle", Infinit::MaterialParameterType::Bool, &m_UseAlbedo));
		mat->AddParameter(new Infinit::MaterialParameter("u_AlbedoColor", Infinit::MaterialParameterType::Color3, &m_AlbedoColor));
		mat->AddTexture("u_NormalTexture", Infinit::Texture2D::Create(Infinit::TextureFormat::RGB, 1, 1));
		mat->AddParameter(new Infinit::MaterialParameter("u_NormalTexToggle", Infinit::MaterialParameterType::Bool, &m_UseNormal));
		mat->AddTexture("u_MetalnessTexture", Infinit::Texture2D::Create(Infinit::TextureFormat::RGB, 1, 1));
		mat->AddParameter(new Infinit::MaterialParameter("u_MetalnessTexToggle", Infinit::MaterialParameterType::Bool, &m_UseMetalness));
		mat->AddParameter(new Infinit::MaterialParameter("u_Metalness", Infinit::MaterialParameterType::Float, &m_Metalness));
		mat->AddTexture("u_RoughnessTexture", Infinit::Texture2D::Create(Infinit::TextureFormat::RGB, 1, 1));
		mat->AddParameter(new Infinit::MaterialParameter("u_RoughnessTexToggle", Infinit::MaterialParameterType::Bool, &m_UseRoughness));
		mat->AddParameter(new Infinit::MaterialParameter("u_Roughness", Infinit::MaterialParameterType::Float, &m_Roughness));
		mat->AddTexture("u_EnvRadianceTex", Infinit::TextureCube::Create("Arches_E_PineTree_Radiance.tga"));
		mat->AddTexture("u_EnvIrradianceTex", Infinit::TextureCube::Create("Arches_E_PineTree_Irradiance.tga"));
		mat->AddTexture("u_BRDFLUTTexture", Infinit::Texture2D::Create("BRDF_LUT.tga"));
		mat->AddParameter(new Infinit::MaterialParameter("u_RadiancePrefilter", Infinit::MaterialParameterType::Float, &m_RadianceFilter));
		mat->AddParameter(new Infinit::MaterialParameter("u_EnvMapRotation", Infinit::MaterialParameterType::Float, &m_EnvMapRotation));
		m_Instance->Material = mat;

		Infinit::GameObject* go = new Infinit::GameObject("Test Go", Infinit::Transform({0.0f, 4.0f, 0.0f}));
		go->StaticMesh = m_Instance;
		go->AddChild((new Infinit::GameObject("Child"))->AddChild(new Infinit::GameObject("Child 3")))->AddChild(new Infinit::GameObject("Child 2"));
		AddGameObject(go);
		
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate() override
	{
		m_Camera->Update();
	}

	virtual void OnRender() override
	{
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Light");
		ImGui::SliderFloat3("Direction", &m_Light->Direction[0], -360.0f, 360.0f);
		ImGui::SliderFloat3("Radiance", &m_Light->Radiance[0], 0.0f, 1.0f);
		ImGui::End();

		ImGui::Begin("Material");
		m_Instance->Material->DrawImGui();
		ImGui::End();
		
		ImGui::Begin("GameObject");
		for (Infinit::GameObject* go : m_GameObjects)
			go->DrawImGui();
		ImGui::End();

		Scene->DrawImGui();
	}

	virtual void OnEvent(Infinit::Event& e) override
	{
		if (e.GetEventType() == Infinit::EventType::KeyPressed)
		{
			Infinit::KeyPressedEvent& keyPress = (Infinit::KeyPressedEvent&)e;
			if (keyPress.GetKeyCode() == IN_KEY_R && keyPress.GetMods() == IN_MOD_CONTROL)
			{
				m_Shader->Reload();
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
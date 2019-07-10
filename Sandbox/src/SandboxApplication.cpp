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
	glm::vec3 m_AlbedoColor = {0.5f, 0.5f, 0.5f};
	float m_Metalness = 0.5f;
	float m_Roughness = 0.5f;
	Infinit::LightMap m_LightMap{ {{0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f}} };
public:
	TestLayer()
		: Infinit::Layer("Test Layer")
	{

	} 



	virtual void OnAttach() override
	{
		m_Camera = new Infinit::Camera(glm::perspective(65.0f, 16.0f / 9.0f, 0.0001f, 10000.0f));
		Scene->SetActiveCamera(m_Camera);
		Scene->SetLightMap(m_LightMap);
		m_Shader = Infinit::Shader::Create("pbr.shader");
		m_Mesh.reset(new Infinit::Mesh("cerberus.fbx"));

		m_Instance.reset(new Infinit::MeshInstance(m_Mesh));
		std::shared_ptr<Infinit::Material> mat = std::make_shared<Infinit::Material>(Infinit::Material(m_Shader));
		//mat->AddTexture("u_AlbedoTexture", Infinit::Texture2D::Create("cerberus_A.png"));
		//mat->AddTexture("u_NormalTexture", Infinit::Texture2D::Create("cerberus_N.png"));
		//mat->AddTexture("u_MetalnessTexture", Infinit::Texture2D::Create("cerberus_M.png"));
		//mat->AddTexture("u_RoughnessTexture", Infinit::Texture2D::Create("cerberus_R.png"));
		mat->AddTexture("u_EnvRadianceTex", Infinit::TextureCube::Create("Arches_E_PineTree_Radiance.tga"));
		mat->AddTexture("u_EnvIrradianceTex", Infinit::TextureCube::Create("Arches_E_PineTree_Irradiance.tga"));
		mat->AddTexture("u_BRDFLUTTexture", Infinit::Texture2D::Create("BRDF_LUT.tga"));
		mat->AddParameter(new Infinit::MaterialParameter("u_AlbedoTexToggle", Infinit::ShaderDataType::Bool, &m_UseAlbedo));
		mat->AddParameter(new Infinit::MaterialParameter("u_MetalnessTexToggle", Infinit::ShaderDataType::Bool, &m_UseMetalness));
		mat->AddParameter(new Infinit::MaterialParameter("u_RoughnessTexToggle", Infinit::ShaderDataType::Bool, &m_UseRoughness));
		mat->AddParameter(new Infinit::MaterialParameter("u_NormalTexToggle", Infinit::ShaderDataType::Bool, &m_UseNormal));
		mat->AddParameter(new Infinit::MaterialParameter("u_RadiancePrefilter", Infinit::ShaderDataType::Float, &m_RadianceFilter));
		mat->AddParameter(new Infinit::MaterialParameter("u_AlbedoColor", Infinit::ShaderDataType::Float3, &m_AlbedoColor));
		mat->AddParameter(new Infinit::MaterialParameter("u_Metalness", Infinit::ShaderDataType::Float, &m_Metalness));
		mat->AddParameter(new Infinit::MaterialParameter("u_Roughness", Infinit::ShaderDataType::Float, &m_Roughness));
		m_Instance->Material = mat;

		Infinit::GameObject* go = new Infinit::GameObject("Test Go", Infinit::Transform({0.0f, 4.0f, 0.0f}));
		go->StaticMesh = m_Instance;
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
		ImGui::SliderFloat3("Direction", &m_LightMap[0].Direction[0], -1.0f, 1.0f, "%.0025f", 0.1f);
		ImGui::SliderFloat3("Radiance", &m_LightMap[0].Radiance[0], 0.0f, 1.0f, "%.0025f", 0.1f);
		ImGui::End();

		ImGui::Begin("Material");
		ImGui::Checkbox("Use Albedo", &m_UseAlbedo);
		ImGui::Checkbox("Use Normal", &m_UseNormal);
		ImGui::Checkbox("Use Metalness", &m_UseMetalness);
		ImGui::Checkbox("Use Roughness", &m_UseRoughness);
		ImGui::SliderFloat("Radiance Filter", &m_RadianceFilter, 0.0f, 1.0f);
		ImGui::ColorEdit3("Albedo Color", &m_AlbedoColor[0]);
		ImGui::SliderFloat("Metalness", &m_Metalness, 0.0f, 1.0f);
		ImGui::SliderFloat("Roughness", &m_Roughness, 0.0f, 1.0f);
		ImGui::End();
		
		ImGui::Begin("GameObject");
		for (Infinit::GameObject* go : m_GameObjects)
			go->DrawImGui();
		ImGui::End();
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
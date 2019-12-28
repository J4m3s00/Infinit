#include "inpch.h"

class TestLayer : public Infinit::Layer
{
public:
	TestLayer()
		: Infinit::Layer("TestLayer")
	{


	}

	virtual void OnAttach() override
	{
		SetupRayMarchingTest();
	}

	void SetupCubeWithBasicMaterial()
	{
		using namespace Infinit;
		ResourceLoader& resourceLoader = Application::Get().GetResourceLoader();

		//Create new gameObject
		GameObject* gameObject = new Infinit::GameObject("TEst GameObject");
		AddGameObject(gameObject);

		//Add a mesh Component
		MeshComponent* meshComponent = gameObject->AddComponent<MeshComponent>();


		//Get the mesh
		std::shared_ptr<Mesh> usedMesh;
		while (resourceLoader.ResourceExist("res/cube.fbx", ResourceNode::Type::MESH) && !usedMesh) {
			usedMesh = resourceLoader.GetResource<Mesh>("res/cube.fbx");
		}
		//Set the mesh to the meshcomponent meshinstance
		meshComponent->m_Mesh = usedMesh;


		//Create the material
		std::shared_ptr<Material> material = std::make_shared<Material>("TestMaterial");
		resourceLoader.AddNotSavedResource(material);
		//Set the material to the meshcomponent
		meshComponent->UsedMaterial = new MaterialInstance(material);

		//Get the shader
		std::shared_ptr<Shader> materialShader;
		while (resourceLoader.ResourceExist("res/shaders/basic.shader", ResourceNode::Type::SHADER) && !materialShader) {
			materialShader = resourceLoader.GetResource<Shader>("res/shaders/basic.shader");
		}
		//Set the shader to the material
		material->SetShader(materialShader);
		meshComponent->UsedMaterial->UpdateShader();

		//Get the shader paramaters
		meshComponent->UsedMaterial->AddParameter("colorTex");
		meshComponent->UsedMaterial->AddParameter("textureTiling");

		//Get the texture
		std::shared_ptr<Texture2D> colorTex;
		while (resourceLoader.ResourceExist("res/textures/Pebbles_012_SD/Pebbles_012_baseColor.jpg", ResourceNode::Type::TEXTURE) && !colorTex) {
			colorTex = resourceLoader.GetResource<Texture2D>("res/textures/Pebbles_012_SD/Pebbles_012_baseColor.jpg");
		}
		//Set the texture
		meshComponent->UsedMaterial->GetParameter<Texture2D>("colorTex")->Texture = colorTex;
		//Set the texturetiling
		meshComponent->UsedMaterial->GetParameter<float>("textureTiling")->Value = 0.5f;
	}
	
	void SetUpCubeWithPbrMaterial()
	{
		using namespace Infinit;
		ResourceLoader& resourceLoader = Application::Get().GetResourceLoader();

		//Create new gameObject
		GameObject* gameObject = new Infinit::GameObject("PBR GameObject");
		AddGameObject(gameObject);

		//Add a mesh Component
		MeshComponent* meshComponent = gameObject->AddComponent<MeshComponent>();


		//Get the mesh
		std::shared_ptr<Mesh> usedMesh;
		while (resourceLoader.ResourceExist("res/cube.fbx", ResourceNode::Type::MESH) && !usedMesh) {
			usedMesh = resourceLoader.GetResource<Mesh>("res/cube.fbx");
		}
		//Set the mesh to the meshcomponent meshinstance
		meshComponent->m_Mesh = usedMesh;


		//Create the material
		std::shared_ptr<Material> material = std::make_shared<Material>("PbrMaterial");
		resourceLoader.AddNotSavedResource(material);
		//Set the material to the meshcomponent
		meshComponent->UsedMaterial = new MaterialInstance(material);

		//Get the shader
		std::shared_ptr<Shader> materialShader;
		while (resourceLoader.ResourceExist("res/shaders/pbr.shader", ResourceNode::Type::SHADER) && !materialShader) {
			materialShader = resourceLoader.GetResource<Shader>("res/shaders/pbr.shader");
		}
		//Set the shader to the material
		material->SetShader(materialShader);
		meshComponent->UsedMaterial->UpdateShader();

		/////////////////////////////
		////LOAD ALL THE TEXTURES////

		std::shared_ptr<TextureCube> envRadianceTex;
		while (resourceLoader.ResourceExist("res/lightmaps/Arches_E_PineTree_Radiance.cubemap", ResourceNode::Type::CUBEMAP) && !envRadianceTex) {
			envRadianceTex = resourceLoader.GetResource<TextureCube>("res/lightmaps/Arches_E_PineTree_Radiance.cubemap");
		}

		std::shared_ptr<TextureCube> envIrradianceTex;
		while (resourceLoader.ResourceExist("res/lightmaps/Arches_E_PineTree_Irradiance.cubemap", ResourceNode::Type::CUBEMAP) && !envIrradianceTex) {
			envIrradianceTex = resourceLoader.GetResource<TextureCube>("res/lightmaps/Arches_E_PineTree_Irradiance.cubemap");
		}

		std::shared_ptr<Texture2D> brdf_LUT;
		while (resourceLoader.ResourceExist("res/lightmaps/BRDF_LUT.tga", ResourceNode::Type::TEXTURE) && !brdf_LUT) {
			brdf_LUT = resourceLoader.GetResource<Texture2D>("res/lightmaps/BRDF_LUT.tga");
		}
		
		std::shared_ptr<Texture2D> albedo;
		while (resourceLoader.ResourceExist("res/textures/Pebbles_012_SD/Pebbles_012_baseColor.jpg", ResourceNode::Type::TEXTURE) && !albedo) {
			albedo = resourceLoader.GetResource<Texture2D>("res/textures/Pebbles_012_SD/Pebbles_012_baseColor.jpg");
		}

		std::shared_ptr<Texture2D> normal;
		while (resourceLoader.ResourceExist("res/textures/Pebbles_012_SD/Pebbles_012_normal.jpg", ResourceNode::Type::TEXTURE) && !normal) {
			normal = resourceLoader.GetResource<Texture2D>("res/textures/Pebbles_012_SD/Pebbles_012_normal.jpg");
		}

		std::shared_ptr<Texture2D> metalness;
		while (resourceLoader.ResourceExist("res/textures/Pebbles_012_SD/Pebbles_012_ambientOcclusion.jpg", ResourceNode::Type::TEXTURE) && !metalness) {
			metalness = resourceLoader.GetResource<Texture2D>("res/textures/Pebbles_012_SD/Pebbles_012_ambientOcclusion.jpg");
		}

		std::shared_ptr<Texture2D> roughness;
		while (resourceLoader.ResourceExist("res/textures/Pebbles_012_SD/Pebbles_012_roughness.jpg", ResourceNode::Type::TEXTURE) && !roughness) {
			roughness = resourceLoader.GetResource<Texture2D>("res/textures/Pebbles_012_SD/Pebbles_012_roughness.jpg");
		}

		////////////////////////////////////////

		//Get the shader paramaters
		meshComponent->UsedMaterial->AddParameter("u_EnvRadianceTex");
		meshComponent->UsedMaterial->GetParameter<TextureCube>("u_EnvRadianceTex")->Texture = envRadianceTex;
		meshComponent->UsedMaterial->AddParameter("u_EnvIrradianceTex");
		meshComponent->UsedMaterial->GetParameter<TextureCube>("u_EnvIrradianceTex")->Texture = envIrradianceTex;
		meshComponent->UsedMaterial->AddParameter("u_BRDFLUTTexture");
		meshComponent->UsedMaterial->GetParameter<Texture2D>("u_BRDFLUTTexture")->Texture = brdf_LUT;
		meshComponent->UsedMaterial->AddParameter("u_AlbedoTexture");
		meshComponent->UsedMaterial->GetParameter<Texture2D>("u_AlbedoTexture")->Texture = albedo;
		meshComponent->UsedMaterial->AddParameter("u_NormalTexture");
		meshComponent->UsedMaterial->GetParameter<Texture2D>("u_NormalTexture")->Texture = normal;
		meshComponent->UsedMaterial->AddParameter("u_MetalnessTexture");
		meshComponent->UsedMaterial->GetParameter<Texture2D>("u_MetalnessTexture")->Texture = metalness;
		meshComponent->UsedMaterial->AddParameter("u_RoughnessTexture");
		meshComponent->UsedMaterial->GetParameter<Texture2D>("u_RoughnessTexture")->Texture = roughness;
		meshComponent->UsedMaterial->AddParameter("u_AlbedoTexToggle");
		meshComponent->UsedMaterial->GetParameter<bool>("u_AlbedoTexToggle")->Value = true;
		meshComponent->UsedMaterial->AddParameter("u_NormalTexToggle");
		meshComponent->UsedMaterial->GetParameter<bool>("u_NormalTexToggle")->Value = true;
		meshComponent->UsedMaterial->AddParameter("u_MetalnessTexToggle");
		meshComponent->UsedMaterial->GetParameter<bool>("u_MetalnessTexToggle")->Value = true;
		meshComponent->UsedMaterial->AddParameter("u_RoughnessTexToggle");
		meshComponent->UsedMaterial->GetParameter<bool>("u_RoughnessTexToggle")->Value = true;
	}

	


	void SetupRayMarchingTest()
	{
		static std::vector<Infinit::Mesh::Vertex> planeVertex = {
		{{-100.0f, -100.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {}, {}},
		{{-100.0f,  100.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {}, {}},
		{{ 100.0f,  100.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
		{{ 100.0f, -100.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {}, {}}
		};

		static std::vector<Infinit::Mesh::Index> planeIndices = {
			{0, 1, 2},
			{2, 3, 0}
		};


		using namespace Infinit;
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(planeVertex, planeIndices);

		ResourceLoader& resourceLoader = Application::Get().GetResourceLoader();

		GameObject* gameObject = new GameObject("Hello World");

		MeshComponent* meshComponent = gameObject->AddComponent<MeshComponent>();
		meshComponent->m_Mesh = mesh;

		std::shared_ptr<Shader> shader;
		while (resourceLoader.ResourceExist("res/shaders/RayMarching.shader", ResourceNode::Type::SHADER) && !shader) {
			shader = resourceLoader.GetResource<Shader>("res/shaders/RayMarching.shader");
		}
		std::shared_ptr<Material> material = std::make_shared<Material>("Ray Marching Material");
		resourceLoader.AddNotSavedResource(material);
		material->SetShader(shader);

		meshComponent->UsedMaterial = new MaterialInstance(material);

		AddGameObject(gameObject);
	}
};

class Sandbox : public Infinit::Application
{
public:
	Sandbox()
		: Infinit::Application("Sandbox", Infinit::RendererAPI::Type::OpenGL)
	{
		using namespace Infinit;
		Scene* scene = new Scene("Hello World");
		scene->PushLayer(new TestLayer());
		SetActiveScene(scene);
	}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}

int main(int argc, char** argv)
{
	Infinit::Application::LaunchArgumentCount = argc;
	Infinit::Application::LaunchArguments = argv;
	Infinit::Log::Init();
	IN_CORE_TRACE("Log initialized!");

	auto app = Infinit::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

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
		using namespace Infinit;
		ResourceLoader& resourceLoader = Application::Get().GetResourceLoader();

		//Create new gameObject
		GameObject* gameObject = new Infinit::GameObject("TEst GameObject");
		AddGameObject(gameObject);

		//Add a mesh Component
		MeshComponent* meshComponent = gameObject->AddComponent<MeshComponent>();


		//Get the mesh
		std::shared_ptr<Mesh> usedMesh;
		while (resourceLoader.ResourceExist("res/cerberus.fbx", ResourceNode::Type::MESH) && !usedMesh) {
			usedMesh = resourceLoader.GetResource<Mesh>("res/cerberus.fbx");
		}
		//Set the mesh to the meshcomponent meshinstance
		meshComponent->m_Instance = new MeshInstance(usedMesh);


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

#include <Infinit.h>
#include <imgui.h>

class TetsLay : public Infinit::Layer
{
private:


	Infinit::FrameBuffer* m_FrameBuffer = nullptr;
public:
	TetsLay()
		: Infinit::Layer("FrameBuffer Layer")
	{

	}

	virtual void OnAttach() override
	{
		m_FrameBuffer = Infinit::FrameBuffer::Create(Infinit::Application::Get().GetWindow().GetWidth(), Infinit::Application::Get().GetWindow().GetHeight(), Infinit::FramebufferFormat::RGBA16F);
	}

	virtual void OnDetach() override
	{
		if (m_FrameBuffer) delete m_FrameBuffer;
	}

	virtual void OnEvent(Infinit::Event& e) override
	{
		Infinit::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Infinit::AppRenderEvent>(IN_BIND_EVENT_FN(TetsLay::Render));
		dispatcher.Dispatch<Infinit::AppUpdateEvent>(IN_BIND_EVENT_FN(TetsLay::Update));
		dispatcher.Dispatch<Infinit::AppImGuiRenderEvent>(IN_BIND_EVENT_FN(TetsLay::RenderImGui));

		if (!e.Handled)
		{
			Infinit::Layer::OnEvent(e);
		}
	}

	bool Render(Infinit::AppRenderEvent& e)
	{
		m_FrameBuffer->Bind();
		Infinit::RendererAPI::s_Instance->Clear();
		
		for (Infinit::GameObject* go : m_GameObjects)
		{
			go->OnEvent(e);
		}

		m_FrameBuffer->Unbind();
		return false;
	}

	bool Update(Infinit::AppUpdateEvent& e)
	{

		return false;
	}

	bool RenderImGui(Infinit::AppImGuiRenderEvent& e)
	{
		ImGui::Begin("Viewport##RenderView");
		auto viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*) m_FrameBuffer->GetColorAttachment(), viewportSize);
		ImGui::End();
		return false;
	}
};

class Sandbox : public Infinit::Application
{
private:
	Infinit::Scene scene;
public:
	Sandbox()
		: Application("Infinit Engine", Infinit::RendererAPI::Type::OpenGL), scene("Test Scene")
	{
		scene.PushLayer(new TetsLay());
		SetActiveScene(&scene);
	}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}
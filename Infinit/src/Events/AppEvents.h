#pragma once
namespace Infinit {

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent(const Transform& transform)
			: m_Transform(transform)
		{}

		inline const Transform& GetTransform() const { return m_Transform; }

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(AppRender);
	private:
		Transform m_Transform;
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent(float deltaTime)
			: m_DeltaTime(deltaTime)
		{}

		inline const float GetDeltaTime() const { return m_DeltaTime; }

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(AppUpdate);
	private:
		float m_DeltaTime;
	};

	class AppImGuiRenderEvent : public Event
	{
	public:
		AppImGuiRenderEvent() {}

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(AppImGuiRender);
	};

}
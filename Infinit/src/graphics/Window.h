#pragma once
struct GLFWwindow;

namespace Infinit {

	struct WindowProp
	{
		WindowProp(const string& title, uint width, uint height)
			: title(title), width(width), height(height)
		{}
		string title;
		uint width;
		uint height;
	};

	class Window
	{
	public:
		using EventCallback = std::function<void(Event&)>;
		Window(const WindowProp&  prop);
		~Window();

		inline GLFWwindow* GetNativeWindow() const { return m_Window; }

		void Update() const;
		bool IsClosed() const;

		inline const uint GetWidth() const { return m_Data.width; }
		inline const uint GetHeight() const { return m_Data.height; }

		void SetEventCallback(const EventCallback& callback) { m_Data.callback = callback; }
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			string title;
			uint width;
			uint height;
			EventCallback callback;
		};
		WindowData m_Data;
	};

}
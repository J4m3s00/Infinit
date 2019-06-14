#pragma once

#include "Core/Core.h"
#include "Event.h"

namespace Infinit {

	class INFINIT_API KeyboardEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		inline int GetScancode() const { return m_Scancode; }
		inline int GetMods() const { return m_Mods; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyboardEvent(int keyCode, int scancode, int mods)
			: m_KeyCode(keyCode), m_Scancode(scancode), m_Mods(mods)
		{

		}
	
		int m_KeyCode,
			m_Scancode,
			m_Mods;
	};

	class INFINIT_API KeyPressedEvent : public KeyboardEvent
	{
	public:
		KeyPressedEvent(int keyCode, int scancode,int mods)
			: KeyboardEvent(keyCode, scancode, mods) {}

		string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " | Scancode: " << m_Scancode << " | Mods: " << m_Mods;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class INFINIT_API KeyDownEvent : public KeyboardEvent
	{
	public:
		KeyDownEvent(int keyCode, int scancode, int mods)
			: KeyboardEvent(keyCode, scancode, mods) {}

		string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyDownEvent: " << m_KeyCode << " | Scancode: " << m_Scancode << " | Mods: " << m_Mods;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyDown)
	};

	class INFINIT_API KeyReleasedEvent : public KeyboardEvent
	{
	public:
		KeyReleasedEvent(int keyCode, int scancode, int mods)
			: KeyboardEvent(keyCode, scancode, mods) {}

		string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode << " | Scancode: " << m_Scancode <<  " | Mods: " << m_Mods;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class INFINIT_API KeyTypedEvent : KeyboardEvent
	{
	public:
		KeyTypedEvent(int keyCode, int scancode,int mods)
			: KeyboardEvent(keyCode, scancode, mods) {}

		string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class INFINIT_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float xPos, float yPos)
			: m_XPos(xPos), m_YPos(yPos)
		{}

		inline float GetXPos() const { return m_XPos; }
		inline float GetYPos() const { return m_YPos; }

		string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << m_XPos << ", " << m_YPos << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseMoved)
	private:
		float m_XPos,
			m_YPos;
	};

	class INFINIT_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset)
		{}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << m_XOffset << ", " << m_YOffset << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseScrolled)
	private:
		float m_XOffset,
			m_YOffset;
	};

	class INFINIT_API MouseButtonEvent : public Event
	{
	public:
		inline int GetButton() const { return m_Button; }
		inline int GetMods() const { return m_Mods; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button, int mods)
			: m_Button(button), m_Mods(mods)
		{}

		int m_Button,
			m_Mods;
	};

	class INFINIT_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button, int mods)
			: MouseButtonEvent(button, mods)
		{}

		string ToString() const
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button << " | Mods: " << m_Mods;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class INFINIT_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button, int mods)
			: MouseButtonEvent(button, mods)
		{}

		string ToString() const
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button << " | Mods: " << m_Mods;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}
#pragma once

namespace Infinit {
	
	class Object;

	class Property : public Serializable
	{
	public:
		Property(const string& name, Object* attachedTo);

		const string& GetPropertyName() const { return m_PropertyName; }

	protected:
		string m_PropertyName;
		Object* m_AttachedObject;
	};

	template <typename T>
	class ValueProperty : public Property
	{
	public:
		ValueProperty(const string& name, Object* attachedTo, const T& defaultValue)
			: Property(name, attachedTo), m_DefaultValue(defaultValue), m_Value(defaultValue)
		{
		}

		void SetValue(const T& value)
		{
			m_Value = value;
		}

		const T& GetValue() const
		{
			return m_Value;
		}

		void Serialize(json& json) const
		{
			json[m_PropertyName + "_Default"] = JsonHelper::ConvertValue(m_DefaultValue);
			json[m_PropertyName] = JsonHelper::ConvertValue(m_Value);
		}

		void Deserialize(const json& json)
		{
			JsonHelper::ConvertObject(json[m_PropertyName], m_Value);
			JsonHelper::ConvertObject(json[m_PropertyName + "_Default"], m_DefaultValue);
		}

	private:
		T m_DefaultValue;
		T m_Value;
	};

}
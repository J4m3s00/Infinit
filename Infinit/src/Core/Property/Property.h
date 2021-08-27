#pragma once

namespace Infinit {
	
	class Object;

	class Property
	{
	public:
		Property(const string& name, Object* attachedTo);

		const string& GetPropertyName() const { return m_PropertyName; }
		virtual void Serialize(json& json, bool writeDefaults = false) const = 0;
		virtual void Deserialize(const json& json) = 0;
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

		void Serialize(json& json, bool writeDefaults = false) const
		{
			if (writeDefaults)
			{
				json[m_PropertyName + "_Default"] = JsonHelper::ConvertValue(m_DefaultValue);
			}
			json[m_PropertyName] = JsonHelper::ConvertValue(m_Value);
		}

		void Deserialize(const json& json)
		{
			JsonHelper::ConvertObject(json[m_PropertyName], m_Value);
			if (JsonHelper::HasObjectProperty((m_PropertyName + "_Default").c_str(), json))
			{
				JsonHelper::ConvertObject(json[m_PropertyName + "_Default"], m_DefaultValue);
			}
		}

		T* operator -> ()
		{
			return &m_Value;
		}

	private:
		T m_DefaultValue;
		T m_Value;
	};

}
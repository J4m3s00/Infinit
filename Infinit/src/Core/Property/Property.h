#pragma once

namespace Infinit {

	template <typename T>
	class Property
	{
	public:
		Property(const string& name, const T& defaultValue)
			: m_PropertyName(name), m_DefaultValue(defaultValue), m_Value(defaultValue)
		{

		}

		void Serialize(json& json)
		{
			json[m_PropertyName + "_Default"] = JsonHelper::ConvertValue(m_DefaultValue);
			json[m_PropertyName] = JsonHelper::ConvertValue(m_Value);
		}

		void Deserialize(const json& json)
		{
			m_Value = json[m_PropertyName];
			m_DefaultValue = json[m_PropertyName + "_Default"];
		}

	private:
		string m_PropertyName;
		T m_DefaultValue;
		T m_Value;
	};

}
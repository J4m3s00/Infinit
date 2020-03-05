#pragma once

namespace Infinit {
	class Object
	{
	public:
		Object(const string& name);

		virtual void Serialize(json& js_object) = 0;
		virtual void Deserialize(const json& js_object) = 0;
	private:
		UUID m_Uuid;
		string m_Name;
	};
}
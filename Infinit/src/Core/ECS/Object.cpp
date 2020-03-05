#include "inpch.h"

namespace Infinit {

	Object::Object(const string& name, const UUID& uuid)
		: m_Name(name), m_Uuid(uuid)
	{

	}

	void Object::Serialize(json& js_object) const
	{
		this->OnSerialize(js_object);

		for (Property* p : m_PropertyList) 
		{
			p->Serialize(js_object);
		}
	}

	void Object::Deserialize(const json& js_object)
	{
		this->OnDeserialize(js_object);

		for (Property* p : m_PropertyList)
		{
			p->Deserialize(js_object);
		}
	}

	void Object::AddProperty(Property* prop)
	{
		m_PropertyList.push_back(prop);
	}
}
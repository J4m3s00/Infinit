#include "inpch.h"

namespace Infinit {

	Property::Property(const string& name, Object* attachedTo)
		: m_PropertyName(name), m_AttachedObject(attachedTo)
	{
		m_AttachedObject->AddProperty(this);
	}

}
#pragma once

namespace Infinit {

	class Serializable
	{
	public:
		virtual void Serialize(json& json) const = 0;
		virtual void Deserialize(const json& json) = 0;
	};

}
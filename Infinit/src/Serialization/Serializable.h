#pragma once

namespace Infinit {

	class Serializable
	{
	public:
		virtual json Serialize() const = 0;
		virtual void Deserialize(const json& json) = 0;
	};

}
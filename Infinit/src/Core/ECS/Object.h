#pragma once

namespace Infinit {
	class Object : public Serializable
	{
	public:
		Object(const string& name, const UUID& uuid = UUID().CreateNew());

		void AddProperty(Property* prop);

		void Serialize(json& json) const override;
		void Deserialize(const json& json) override;
		virtual void OnSerialize(json& json) const = 0;
		virtual void OnDeserialize(const json& json) = 0;
	protected:
		std::vector<Property*> m_PropertyList;
	private:
		UUID m_Uuid;
		string m_Name;
	};
}
#pragma once

namespace Infinit {
	class Object : public Serializable
	{
	public:
		Object(const string& name, const UUID& uuid = UUID().CreateNew());

		const UUID& GetUuid() const { return m_Uuid.GetValue(); }
		void SetName(const string& name) { m_Name.SetValue(name); }
		const string& GetName() const { return m_Name.GetValue();  }

		void AddProperty(Property* prop);

		void Serialize(json& json) const override;
		void Deserialize(const json& json) override;
		virtual void OnSerialize(json& json) const {};
		virtual void OnDeserialize(const json& json) {};
	protected:
		std::vector<Property*> m_PropertyList;
		ValueProperty<UUID> m_Uuid;
		ValueProperty<string> m_Name;
	};
}
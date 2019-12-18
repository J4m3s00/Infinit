#pragma once
namespace Infinit {

    class Resource : public Serializable
    {
    public:
        Resource(const string& filePath, const string& name = "Unknown") : m_FilePath(filePath), m_Name(name) {}
        virtual ~Resource() {}

        virtual json Serialize() const override { return { {"Name", m_Name}, {"FilePath", m_FilePath} }; }
        virtual void Deserialize(const json& json_object) override { m_FilePath = json_object["FilePath"]; m_Name = json_object["Name"]; }

		inline const string& GetFilePath() const { return m_FilePath; }
        inline const string& GetName() const { return m_Name; }

		inline void ChangeName(const string& name) { m_Name = name; }

        virtual bool Reload(const string& filePath) = 0;
    protected:
        string m_FilePath;
        string m_Name;
    };

}
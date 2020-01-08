#pragma once
namespace Infinit {

    class Resource : public Serializable
    {
        friend class ResourceLoader;
    public:
    enum Type : int
    {
        UNKNOWN = 0,
        ENGINE_RESOURCE,
        FOLDER,
        MATERIAL,
        SHADER,
        TEXTURE,
        CUBEMAP,
        MESH
    };
    public:
        Resource(const string& filePath, Type resourceType, const string& name = "Unknown") : m_FilePath(filePath), m_Name(name), m_ResourceType(resourceType) {}
        virtual ~Resource() {}

        virtual json Serialize() const override { return { {"Name", m_Name}, {"FilePath", m_FilePath}, {"Type", m_ResourceType} }; }
        virtual void Deserialize(const json& json_object) override { m_FilePath = json_object["FilePath"]; m_Name = json_object["Name"]; m_ResourceType = json_object["Type"]; }

		inline const string& GetFilePath() const { return m_FilePath; }
        inline const string& GetName() const { return m_Name; }
        inline const Type GetType() const { return m_ResourceType;  }

		inline void ChangeName(const string& name) { m_Name = name; }

        virtual bool Reload(const string& filePath) = 0;

        virtual void ImGuiDraw();
    private:
        Type m_ResourceType;
    protected:
        string m_FilePath;
        string m_Name;
    };

}
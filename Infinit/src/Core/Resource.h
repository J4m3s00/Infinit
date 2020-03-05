#pragma once
namespace Infinit {

    class Resource : public Object
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

        virtual void OnSerialize(json& js_object) const override;
        virtual void OnDeserialize(const json& json_object) override;

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
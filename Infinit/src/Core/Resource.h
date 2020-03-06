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
        Resource(const string& filePath, Type resourceType, const string& name = "Unknown");
        virtual ~Resource() {}

		inline const string& GetFilePath() const { return m_FilePath.GetValue(); }

        inline const Type GetType() const { return (Type) m_ResourceType.GetValue();  }

        virtual bool Reload(const string& filePath) = 0;

        virtual void ImGuiDraw();
    private:
        ValueProperty<int> m_ResourceType;
    protected:
        ValueProperty<string> m_FilePath;
    };

}
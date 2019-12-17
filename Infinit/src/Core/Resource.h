#pragma once
namespace Infinit {

    class Resource
    {
    public:
        Resource(const string& filePath, const string& name = "Unknown") : m_FilePath(filePath), m_Name(name) {}
        virtual ~Resource() {}

		inline const string& GetFilePath() const { return m_FilePath; }
        inline const string& GetName() const { return m_Name; }

		inline void ChangeName(const string& name) { m_Name = name; }

        virtual bool Reload(const string& filePath) = 0;
    protected:
        string m_FilePath;
        string m_Name;
    };

}
#pragma once

#include "Core/Core.h"
#include "Core/Resource.h"

namespace Infinit {

	typedef std::function<void(std::shared_ptr<Resource>)> ResourceLoadFinishFn;

	//Editor funktionality
	class ResourceNode
	{
		friend class ResourceLoader;
	public:
		enum Type : int
		{
			UNKNOWN = 0,
			FOLDER,
			MATERIAL,
			SHADER,
			TEXTURE,
			CUBEMAP,
			MESH
		};
	public:
		ResourceNode(const string& name);
		~ResourceNode();

		void SetType(Type type) { m_NodeType = type; }
		Type GetType() { return m_NodeType; }

		ResourceNode* GetNext() { return m_Next; }
		ResourceNode* GetChild() { return m_Child; }
		ResourceNode* GetParent() { return m_Parent; }
		ResourceNode* GetPreviues() { return m_Prev; }

		const string& GetName() const { return m_Name; }

		void AddChild(ResourceNode* child);
		void AddNext(ResourceNode* next);
		ResourceNode* Find(const string& name);

		template <typename T >
		std::shared_ptr<T> GetResource() { return std::dynamic_pointer_cast<T>(m_Resource); }
		void SetResource(std::shared_ptr<Resource> resource);
	private:
		ResourceNode* FindRec(ResourceNode* n, const string& name);
	protected:
		string m_Name;
		Type m_NodeType;

		ResourceNode* m_Parent;
		ResourceNode* m_Next;
		ResourceNode* m_Prev;
		ResourceNode* m_Child;

		std::shared_ptr<Resource> m_Resource;
	};



	class ResourceLoader
	{
	public:
		ResourceLoader();
		~ResourceLoader();

		void AddResourceToLoad(const string& filePath, bool bottom = false);
		bool ResourceExist(const string& path, ResourceNode::Type resourceType);

		template <typename T>
		std::shared_ptr<T> GetResource(const string& localPath) 
		{
			ResourceNode* result = m_ResourceTree->Find(localPath);
			if (!result) return nullptr;
			return result->GetResource<T>();
		}
		void AddResourceLoadFinishCallback(const string& filePath, ResourceLoadFinishFn callback);

		void ImGuiDraw();
	private:
		std::thread* m_Thread;
		std::mutex m_PushPathMutex;
		std::mutex m_InsertResourceMutex;
		std::vector<string> m_ResourcesToLoad;
		std::vector<std::future<void>> m_Futures;
		std::unordered_map<string, ResourceLoadFinishFn> m_ResourceLoadFinishCallbacks;
		std::atomic<bool> m_Running;
		ResourceNode* m_ResourceTree;
		ResourceNode* m_CurrentNode;
		int m_CurrentMovedFileType = 0;
	};

}
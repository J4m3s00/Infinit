#pragma once

namespace Infinit {

	typedef std::function<void(std::shared_ptr<Resource>)> ResourceLoadFinishFn;

	//Editor funktionality
	class ResourceNode
	{
		friend class ResourceLoader;
	public:
		ResourceNode(const string& name);
		~ResourceNode();

		void SetType(Resource::Type type) { m_NodeType = type; }
		Resource::Type GetType() { return m_NodeType; }

		ResourceNode* GetNext() { return m_Next; }
		ResourceNode* GetChild() { return m_Child; }
		ResourceNode* GetParent() { return m_Parent; }
		ResourceNode* GetPreviues() { return m_Prev; }

		const string& GetName() const { return m_Name; }

		void AddChild(ResourceNode* child);
		void AddNext(ResourceNode* next);
		ResourceNode* Find(const string& name);

		void ForEach(std::function<void(ResourceNode*)> callback);
		const string& GetFullPath() const;

		template <typename T >
		std::shared_ptr<T> GetResource() { return std::dynamic_pointer_cast<T>(m_Resource); }
		void SetResource(std::shared_ptr<Resource> resource);
	private:
		ResourceNode* FindRec(ResourceNode* n, const string& name);
		void ForEachRec(ResourceNode* n, std::function<void(ResourceNode*)> callback);
	protected:
		string m_Name;
		mutable string m_FullPath;
		Resource::Type m_NodeType;

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
		bool ResourceExist(const string& path, Resource::Type resourceType);
		void LoadCompleteResourceTree();

		template <typename T>
		std::shared_ptr<T> GetResource(const string& localPath) 
		{
			ResourceNode* result = m_ResourceTree->Find(ChangeFileEnding(localPath, ".inr"));
			if (!result)
			{
				result = m_ResourceTree->Find(localPath);
				if (!result)
				{
					IN_CORE_ERROR("Could not find resource \"{0}\"!", localPath);
					return nullptr;
				}
			}
			return result->GetResource<T>();
		}

		void AddNotSavedResource(std::shared_ptr<Resource> resource);
		void SaveResource(Resource* resource);

		bool ShowFileDialog(Resource::Type filter, ResourceNode** currentDirectory, bool* open = NULL);
		void ImGuiDraw();
	private:
		string ChangeFileEnding(const string& path, const string& fileEnding);
	private:
		std::vector<string> m_ResourcesToLoad;
		std::vector<std::future<void>> m_Futures;
		ResourceNode* m_ResourceTree;
		ResourceNode* m_CurrentNode;
		ResourceNode* m_LastClickedResource;
		ResourceNode* m_CurrentFileDialogNode;
		std::vector<std::shared_ptr<Resource>> m_NotSaved;
	};

}
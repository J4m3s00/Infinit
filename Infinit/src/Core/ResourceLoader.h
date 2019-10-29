#pragma once

#include "Core/Core.h"
#include "Core/Resource.h"

namespace Infinit {

	typedef std::function<void(std::shared_ptr<Resource>)> ResourceLoadFinishFn;

	class ResourceLoader
	{
	public:
		ResourceLoader();
		~ResourceLoader();

		void AddResourceToLoad(const string& filePath, bool bottom = false);
		std::shared_ptr<Resource> GetResource(const string& localPath);
		void AddResourceLoadFinishCallback(const string& filePath, ResourceLoadFinishFn callback);
	private:
		void Tick();
		void LoadResource(const string& filePath);
	private:
		std::thread* m_Thread;
		std::mutex m_PushPathMutex;
		std::mutex m_InsertResourceMutex;
		std::vector<string> m_ResourcesToLoad;
		std::unordered_map<string, ResourceLoadFinishFn> m_ResourceLoadFinishCallbacks;
		std::atomic<bool> m_Running;
		std::vector<std::shared_ptr<Resource>> m_ResourceCache;
	};

}
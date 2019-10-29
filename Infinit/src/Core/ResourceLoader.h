#pragma once

#include "Core/Core.h"
#include "Core/Resource.h"

namespace Infinit {

	class ResourceLoader
	{
	public:
		ResourceLoader();
		~ResourceLoader();

		void AddResourceToLoad(const string& filePath, bool bottom = false);
		std::shared_ptr<Resource> GetResource(const string& localPath);
	private:
		void Tick();
		void LoadResource(const string& filePath);
	private:
		std::thread* m_Thread;
		std::mutex m_PushPathMutex;
		std::mutex m_InsertResourceMutex;
		std::vector<string> m_ResourcesToLoad;
		std::atomic<bool> m_Running;
		std::vector<std::shared_ptr<Resource>> m_ResourceCache;
	};

}
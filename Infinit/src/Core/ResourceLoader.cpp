#include "inpch.h"
#include "ResourceLoader.h"

#include "Application.h"
#include "Resource.h"
#include "Core/Log.h"

#include "graphics/Texture.h"
#include "Material.h"
#include "graphics/Shader.h"
#include "graphics/Mesh.h"

#ifdef IN_PLATFORM_WINDOWS
#include <filesystem>
#endif

namespace Infinit {

	ResourceLoader::ResourceLoader()
	{
		m_Thread = new std::thread(&ResourceLoader::Tick, this);
	}

	ResourceLoader::~ResourceLoader()
	{
		m_Running = false;
		m_Thread->join();
	}

	void ResourceLoader::Tick()
	{
		while (m_Running)
		{
			while (m_ResourcesToLoad.size() > 0)
			{
				LoadResource(m_ResourcesToLoad.back());
				m_ResourcesToLoad.pop_back();
			}
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}

	void ResourceLoader::LoadResource(const string& filePath)
	{
		string absolutePath = filePath;
		string relativPath = filePath;
		std::filesystem::path path(filePath);
		bool hasCallback = m_ResourceLoadFinishCallbacks.find(filePath) != m_ResourceLoadFinishCallbacks.end();

		const string& resourcePath = Application::Get().GetApplicationPath();

		if (path.is_absolute())
		{
			absolutePath = std::filesystem::absolute(path).u8string();
			relativPath = absolutePath.substr(resourcePath.size(), absolutePath.size() - resourcePath.size());

			std::replace(absolutePath.begin(), absolutePath.end(), '\\', '/');
			std::replace(relativPath.begin(), relativPath.end(), '\\', '/');
		}



		std::vector<std::shared_ptr<Resource>>::iterator it = std::find_if(m_ResourceCache.begin(), m_ResourceCache.end(), [&relativPath](const std::shared_ptr<Resource>& r) { return r->GetName() == relativPath; });
		if (it != m_ResourceCache.end())
		{
			return;
		}
		
		//m_Futures.push_back(std::async(std::launch::async, SaveResourceInCache, &m_ResourceCache, relativPath, absolutePath, callback));

		IN_CORE_TRACE("Save resource {0}", filePath);

		string fileEnding = relativPath.substr(relativPath.find_last_of(".") + 1, relativPath.size());
		//Textures
		std::shared_ptr<Resource> result;
		if (fileEnding == "png" || fileEnding == "tga")
		{
			{
				result = std::dynamic_pointer_cast<Resource>(Texture2D::Create(absolutePath));
				result->ChangeName(relativPath);
				std::lock_guard<std::mutex> lock(m_InsertResourceMutex);
				m_ResourceCache.push_back(result);
			}
		}
		//Cubemaps
		else if (fileEnding == "cubemap")
		{
			{
				result = std::dynamic_pointer_cast<Resource>(TextureCube::Create(absolutePath));
				result->ChangeName(relativPath);
				std::lock_guard<std::mutex> lock(m_InsertResourceMutex);
				m_ResourceCache.push_back(result);
			}
		}
		//Materials
		else if (fileEnding == "inm")
		{
			{
				result = std::dynamic_pointer_cast<Resource>(std::shared_ptr <Material>(new Material(absolutePath)));
				result->ChangeName(relativPath);
				std::lock_guard<std::mutex> lock(m_InsertResourceMutex);
				m_ResourceCache.push_back(result);

			}
		}
		//Meshes
		else if (fileEnding == "fbx")
		{
			{
				result = std::dynamic_pointer_cast<Resource>(std::shared_ptr<Mesh>(new Mesh(absolutePath)));
				result->ChangeName(relativPath);
				std::lock_guard<std::mutex> lock(m_InsertResourceMutex);
				m_ResourceCache.push_back(result);

			}
		}
		//Shadersm_Futures
		else if (fileEnding == "shader")
		{
			{
				result = std::dynamic_pointer_cast<Resource>(Shader::Create(absolutePath));
				result->ChangeName(relativPath);
				std::lock_guard<std::mutex> lock(m_InsertResourceMutex);
				m_ResourceCache.push_back(result);
			}
		}
		if (hasCallback)
		{
			m_ResourceLoadFinishCallbacks[filePath](result);
			m_ResourceLoadFinishCallbacks.erase(filePath);
		}
	}

	void ResourceLoader::AddResourceToLoad(const string& path, bool bottom)
	{
		std::lock_guard<std::mutex> lock(m_PushPathMutex);
		if (bottom)
			m_ResourcesToLoad.insert(m_ResourcesToLoad.begin(), path);
		else
			m_ResourcesToLoad.push_back(path);
	}

	std::shared_ptr<Resource> ResourceLoader::GetResource(const string& localPath)
	{
		std::vector<std::shared_ptr<Resource>>::iterator it = std::find_if(m_ResourceCache.begin(), m_ResourceCache.end(), [&localPath](const std::shared_ptr<Resource>& resource) {return resource->GetName() == localPath; });
		if (it != m_ResourceCache.end())
			return *it;
		return nullptr;
	}

	void ResourceLoader::AddResourceLoadFinishCallback(const string& filePath, ResourceLoadFinishFn callback)
	{
		m_ResourceLoadFinishCallbacks.insert({ filePath, callback });
	}

}
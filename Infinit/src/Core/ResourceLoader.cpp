#include "inpch.h"
#include "ResourceLoader.h"

#include "Application.h"
#include "Resource.h"
#include "Core/Log.h"

#include "graphics/Texture.h"
#include "Material.h"
#include "graphics/Shader.h"
#include "graphics/Mesh.h"

#include "Util/StringUtil.h"

#ifdef IN_PLATFORM_WINDOWS
#include <filesystem>
#endif

namespace Infinit {

	static ResourceNode* AddPathToResourceTree(ResourceNode* tree, const string& path);
	std::atomic_bool CancleLoading = false;

	static ResourceNode::Type GetResourceTypeByPath(const string& path)
	{
		size_t dotPos = path.find_last_of(".");
		if (dotPos == string::npos) return ResourceNode::Type::FOLDER;

		string fileEnding = path.substr(dotPos + 1, path.size());

		if (fileEnding == "png" || fileEnding == "tga") return ResourceNode::Type::TEXTURE;
		//Cubemaps
		else if (fileEnding == "cubemap")return ResourceNode::Type::CUBEMAP;
		//Materials
		else if (fileEnding == "inm")return ResourceNode::Type::MATERIAL;
		//Meshes
		else if (fileEnding == "fbx")return ResourceNode::Type::MESH;
		//Shadersm_Futures
		else if (fileEnding == "shader")return ResourceNode::Type::SHADER;

		return ResourceNode::Type::UNKNOWN;
	}

	ResourceLoader::ResourceLoader()
	{
		m_ResourceTree = new ResourceNode("res");
		m_ResourceTree->SetType(ResourceNode::Type::FOLDER);
		m_CurrentNode = m_ResourceTree;
	}

	ResourceLoader::~ResourceLoader()
	{
		CancleLoading = true;
		delete m_ResourceTree;
	}

	static void LoadResource(ResourceNode* resourceTree, string filePath)
	{
		string absolutePath = filePath;
		string relativPath = filePath;
		std::filesystem::path path(filePath);

		const string& resourcePath = Application::Get().GetApplicationPath();

		if (path.is_absolute())
		{
			absolutePath = std::filesystem::absolute(path).u8string();
			relativPath = absolutePath.substr(resourcePath.size(), absolutePath.size() - resourcePath.size());

			std::replace(absolutePath.begin(), absolutePath.end(), '\\', '/');
			std::replace(relativPath.begin(), relativPath.end(), '\\', '/');
		}


		ResourceNode* node = resourceTree->Find(relativPath);
		if (node && node->GetResource<Resource>()) return;
		
		IN_CORE_TRACE("Save resource {0}", filePath);

		ResourceNode::Type resourceType = GetResourceTypeByPath(relativPath);
		//Textures
		std::shared_ptr<Resource> result;
		switch (resourceType)
		{
		case ResourceNode::Type::TEXTURE: 
		{
			result = std::dynamic_pointer_cast<Resource>(Texture2D::Create(absolutePath));
			result->ChangeName(relativPath);
			break;
		}
		case ResourceNode::Type::CUBEMAP:
		{
			result = std::dynamic_pointer_cast<Resource>(TextureCube::Create(absolutePath));
			result->ChangeName(relativPath);
			break;
		}
		case ResourceNode::Type::MATERIAL:
		{
			result = std::dynamic_pointer_cast<Resource>(std::shared_ptr <Material>(new Material(absolutePath)));
			result->ChangeName(relativPath);
			break;
		}
		case ResourceNode::Type::MESH:
		{
			result = std::dynamic_pointer_cast<Resource>(std::shared_ptr<Mesh>(new Mesh(absolutePath)));
			result->ChangeName(relativPath);
			break;
		}
		case ResourceNode::Type::SHADER:
		{
			result = std::dynamic_pointer_cast<Resource>(Shader::Create(absolutePath));
			result->ChangeName(relativPath);
			break;
		}
		}
		if (!CancleLoading)
		{
			if (!node)
			{
				node = AddPathToResourceTree(resourceTree, relativPath);
			}
			node->SetResource(result);
		}
	}

	void ResourceLoader::AddResourceToLoad(const string& path, bool bottom)
	{
		//Make check that path is relativ to res folder
		AddPathToResourceTree(m_ResourceTree, path);
	}

	void ResourceLoader::LoadCompleteResourceTree()
	{
		m_ResourceTree->ForEach([this](ResourceNode* node) 
			{ 
				IN_CORE_INFO("Loading node \"{0}\" from type {1}", node->GetName(), node->GetType()); 
				if (node->GetType() != ResourceNode::Type::FOLDER && node->GetType() != ResourceNode::Type::UNKNOWN) 
				{ 
					m_Futures.push_back(std::async(std::launch::async, LoadResource, m_ResourceTree, node->GetFullPath())); 
				}
			});
	}

	bool ResourceLoader::ResourceExist(const string& path, ResourceNode::Type resourceType)
	{
		if (GetResourceTypeByPath(path) != resourceType) { return false; }
		return m_ResourceTree->Find(path);
	}

	void ResourceLoader::ImGuiDraw()
	{
		ImGui::Begin("Resources##ResourceManager");
		ResourceNode* displayNode = m_CurrentNode;
		
		if (displayNode->GetParent())
		{
			if (ImGui::Button("Back##bbutton"))
			{
				m_CurrentNode = displayNode->GetParent();
				while (m_CurrentNode->GetPreviues())
				{
					m_CurrentNode = m_CurrentNode->GetPreviues();
				}
				displayNode = m_CurrentNode;
			}
		}

		do
		{
			if (displayNode->GetChild())
			{
				if (ImGui::Button(("Open##Open" + displayNode->GetName()).c_str()))
				{
					m_CurrentNode = displayNode->GetChild();
					displayNode = displayNode->GetChild();
					ImGui::End();
					return;
				}
			}
			ImGui::Selectable(displayNode->GetName().c_str());

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("RESOURCE_NODE", (const void*) displayNode, sizeof(ResourceNode));
				ImGui::EndDragDropSource();
			}

			displayNode = displayNode->GetNext();
		} while (displayNode);


		ImGui::End();
	}


	ResourceNode::ResourceNode(const string& name)
		: m_Name(name)
	{
		m_Child = nullptr;
		m_Next = nullptr;
		m_Parent = nullptr;
		m_Prev = nullptr;
	}

	ResourceNode::~ResourceNode()
	{
		if (m_Child)
		{
			delete m_Child;
			m_Child = nullptr;
		}
		if (m_Next)
		{
			delete m_Next;
			m_Next = nullptr;
		}
	}

	void ResourceNode::SetResource(std::shared_ptr<Resource> resource)
	{
		m_Resource = resource;
	}

	void ResourceNode::AddChild(ResourceNode* child)
	{
		if (m_Child) m_Child->AddNext(child);
		else
		{
			m_Child = child;
			child->m_Parent = this;
		}
	}

	void ResourceNode::AddNext(ResourceNode* next)
	{
		if (m_Next) m_Next->AddNext(next);
		else
		{
			m_Next = next;
			m_Next->m_Prev = this;
			m_Next->m_Parent = m_Parent;
		}
	}

	ResourceNode* ResourceNode::Find(const string& name)
	{
		std::vector<string> folders = SplitString(name, "/");
		ResourceNode* result = this;
		for (string& current : folders)
		{
			result = FindRec(result, current);
		}
		return result;
	}

	ResourceNode* ResourceNode::FindRec(ResourceNode* node, const string& name)
	{
		if (node)
		{
			ResourceNode* result = nullptr;
			if (node->m_Name == name) return node;
			if (node->m_Next && (result = FindRec(node->m_Next, name)) != nullptr) return result;
			if (node->m_Child && (result = FindRec(node->m_Child, name)) != nullptr) return result;
		}
		return nullptr;
	}

	void ResourceNode::ForEach(std::function<void(ResourceNode*)> callback)
	{
		ForEachRec(this, callback);
	}

	void ResourceNode::ForEachRec(ResourceNode* node, std::function<void(ResourceNode*)> callback)
	{
		if (node)
		{
			callback(node);
			if (node->m_Next) { ForEachRec(node->m_Next, callback); }
			if (node->m_Child) { ForEachRec(node->m_Child, callback); }
		}
	}

	const string& ResourceNode::GetFullPath() const
	{
		const ResourceNode* current = this;
		m_FullPath.clear();
		bool first = false;
		do
		{
			m_FullPath = current->GetName() + (first ? "/" : "") + m_FullPath;
			first = true;
			current = current->m_Parent;
		} while (current);
		return m_FullPath;
	}

	static std::mutex addResourceTreeMutex;

	ResourceNode* AddPathToResourceTree(ResourceNode* tree, const string& path)
	{
		std::lock_guard<std::mutex> lock(addResourceTreeMutex);
		std::vector<string> folders = SplitString(path, "/");
		ResourceNode* pointer = tree;
		ResourceNode* found = nullptr;
		for (string& current : folders)
		{
			if (!(found = pointer->Find(current)))
			{
				ResourceNode* temp = new ResourceNode(current);
				temp->SetType(GetResourceTypeByPath(current));
				pointer->AddChild(temp);
				pointer = temp;
			}
			else
			{
				pointer = found;
			}
		}
		return pointer;
	}

}
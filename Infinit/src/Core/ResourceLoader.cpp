#include "inpch.h"

namespace Infinit {

	static ResourceNode* AddPathToResourceTree(ResourceNode* tree, const string& path);
	std::atomic_bool CancleLoading = false;

	static Resource::Type GetResourceTypeByPath(const string& path)
	{
		size_t dotPos = path.find_last_of(".");
		if (dotPos == string::npos) return Resource::Type::FOLDER;

		string fileEnding = path.substr(dotPos + 1, path.size());

		if (fileEnding == "png" || fileEnding == "tga" || fileEnding == "jpg") return Resource::Type::TEXTURE;
		//Cubemaps
		else if (fileEnding == "cubemap")return Resource::Type::CUBEMAP;
		//Materials
		else if (fileEnding == "inm")return Resource::Type::MATERIAL;
		//Meshes
		else if (fileEnding == "fbx")return Resource::Type::MESH;
		//Shadersm_Futures
		else if (fileEnding == "shader")return Resource::Type::SHADER;


		//engine intern resource file
		else if (fileEnding == "inr") return Resource::Type::ENGINE_RESOURCE;

		return Resource::Type::UNKNOWN;
	}

	ResourceLoader::ResourceLoader()
	{
		m_ResourceTree = new ResourceNode("res");
		m_ResourceTree->SetType(Resource::Type::FOLDER);
		m_CurrentNode = m_ResourceTree;
		m_CurrentFileDialogNode = m_ResourceTree;
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

		Resource::Type resourceType = GetResourceTypeByPath(relativPath);
		//Textures
		std::shared_ptr<Resource> result;
		switch (resourceType)
		{
		case Resource::Type::TEXTURE:
		{
			result = std::dynamic_pointer_cast<Resource>(Texture2D::Create(absolutePath));
			result->ChangeName(node->GetName());
			break;
		}
		case Resource::Type::CUBEMAP:
		{
			result = std::dynamic_pointer_cast<Resource>(TextureCube::Create(absolutePath));
			result->ChangeName(node->GetName());
			break;
		}
		case Resource::Type::MATERIAL:
		{
			result = std::dynamic_pointer_cast<Resource>(std::make_shared<Material>("", absolutePath));
			break;
		}
		case Resource::Type::MESH:
		{
			result = std::dynamic_pointer_cast<Resource>(std::make_shared<Mesh>(absolutePath));
			result->ChangeName(node->GetName());
			break;
		}
		case Resource::Type::SHADER:
		{
			result = std::dynamic_pointer_cast<Resource>(Shader::Create(absolutePath));
			result->ChangeName(node->GetName());
			break;
		}
		case Resource::Type::ENGINE_RESOURCE:
		{
			std::ifstream i(absolutePath);
			json json_object;
			i >> json_object;

			Resource::Type loadingType = json_object["Type"];

			switch (loadingType)
			{
			case Resource::Type::TEXTURE:
			{
				result = Texture2D::Create(json_object);
				break;
			}
			case Resource::Type::CUBEMAP:
			{
				result = TextureCube::Create(json_object);
				break;
			}
			case Resource::Type::MATERIAL:
			{
				result = std::make_shared<Material>();
				result->Deserialize(json_object);
				break;
			}
			case Resource::Type::MESH:
			{
				result = std::make_shared<Mesh>();
				result->Deserialize(json_object);
				break;
			}
			case Resource::Type::SHADER:
			{
				result = Shader::Create(json_object);
				break;
			}
			}
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
		//Check if engine resource exist
		Resource::Type resType = GetResourceTypeByPath(path);
		if (resType != Resource::Type::ENGINE_RESOURCE && resType != Resource::Type::UNKNOWN && resType != Resource::Type::FOLDER)
		{
			string engineResourcePath = ChangeFileEnding(path, ".inr");
			if (std::filesystem::exists(std::filesystem::path(engineResourcePath)))
			{
				//Dont add the resource if an engine resource exist
				return;
			}
		}

		//Make check that path is relativ to res folder
		AddPathToResourceTree(m_ResourceTree, path);
	}

	void ResourceLoader::SaveResource(Resource* resource)
	{

		if (GetResource<Resource>(resource->GetFilePath()))
		{
			//Change filepath with propper ending
			string resourcePath = resource->GetFilePath();
			Resource::Type resourcePathType = GetResourceTypeByPath(resourcePath);
			if (resourcePathType != Resource::Type::ENGINE_RESOURCE)
			{
				size_t dotPos = resourcePath.find_last_of(".");
				resourcePath = resourcePath.substr(0, dotPos);
				resourcePath += ".inr";
				resource->m_FilePath = resourcePath;
			} 
			json json_resource = resource->Serialize();

			std::ofstream o(resourcePath);
			o << std::setw(4) << json_resource << std::endl;
			resource->Reload(resourcePath);
		}
	}

	string ResourceLoader::ChangeFileEnding(const string& path, const string& fileEnding)
	{
		string resourcePath = path;
		size_t dotPos = resourcePath.find_last_of(".");
		resourcePath = resourcePath.substr(0, dotPos);
		resourcePath += fileEnding; 
		return resourcePath;
	}

	void ResourceLoader::LoadCompleteResourceTree()
	{
		m_ResourceTree->ForEach([this](ResourceNode* node) 
			{ 
				IN_CORE_INFO("Loading node \"{0}\" from type {1}", node->GetName(), node->GetType()); 
				if (node->GetType() != Resource::Type::FOLDER && node->GetType() != Resource::Type::UNKNOWN)
				{ 
					if (node->GetType() != Resource::Type::ENGINE_RESOURCE)
					{
						string engineResourcePath = ChangeFileEnding(node->GetFullPath(), ".inr");
						if (!ResourceExist(engineResourcePath, Resource::Type::ENGINE_RESOURCE))
						{
							m_Futures.push_back(std::async(std::launch::async, LoadResource, m_ResourceTree, node->GetFullPath()));
						}
					}
					else
					{
						m_Futures.push_back(std::async(std::launch::async, LoadResource, m_ResourceTree, node->GetFullPath()));
					}
					
				}
			});
	}

	bool ResourceLoader::ResourceExist(const string& path, Resource::Type resourceType)
	{
		if (GetResourceTypeByPath(path) != resourceType) { return false; }
		return m_ResourceTree->Find(path);
	}

	void ResourceLoader::AddNotSavedResource(std::shared_ptr<Resource> resource)
	{
		m_NotSaved.push_back(resource);
	}

	bool ResourceLoader::ShowFileDialog(Resource::Type filter, ResourceNode** currentDirectory, bool* open)
	{
		if (ImGui::BeginPopupModal("Open File##FileDialog", open))
		{
			ResourceNode* displayNode = m_CurrentFileDialogNode;
			if (displayNode)
			{
				if (displayNode->GetParent())
				{
					if (ImGui::Button("Back##bbutton"))
					{
						m_CurrentFileDialogNode = displayNode->GetParent();
						while (m_CurrentFileDialogNode->GetPreviues())
						{
							m_CurrentFileDialogNode = m_CurrentFileDialogNode->GetPreviues();
						}
						displayNode = m_CurrentFileDialogNode;
					}
				}

				do
				{
					if (displayNode->GetType() != filter)
					{
						displayNode = displayNode->GetNext();
						continue;
					}

					if (ImGui::Button(displayNode->GetName().c_str(), { 64, 32 }))
					{
						*currentDirectory = m_CurrentFileDialogNode;

						m_CurrentFileDialogNode = displayNode->GetChild();

						if (!m_CurrentFileDialogNode) {
							m_CurrentFileDialogNode = new ResourceNode("..");
							m_CurrentFileDialogNode->m_Parent = displayNode;
						}

						ImGui::EndPopup();
						return false;
					}

					displayNode = displayNode->GetNext();
				} while (displayNode);
			}


			if (ImGui::Button("Ok##FileDialog_OK"))
			{
				*currentDirectory = m_CurrentFileDialogNode->GetParent();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return true;
			}


			ImGui::EndPopup();
		}
		return false;
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
			if (ImGui::Selectable(displayNode->GetName().c_str())) {
				m_LastClickedResource = displayNode;
			}

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("RESOURCE_NODE", (const void*) displayNode, sizeof(ResourceNode));
				ImGui::EndDragDropSource();
			}

			displayNode = displayNode->GetNext();
		} while (displayNode);


		ImGui::End();

		if (m_LastClickedResource && m_LastClickedResource->GetResource<Resource>() && (m_LastClickedResource->GetType() != Resource::Type::FOLDER || m_LastClickedResource->GetType() != Resource::Type::UNKNOWN))
		{

			ImGui::Begin("Resource View##ResourceView");
			m_LastClickedResource->GetResource<Resource>().get()->ImGuiDraw();
			ImGui::End();
		}
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
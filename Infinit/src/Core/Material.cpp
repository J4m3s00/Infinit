#include "inpch.h"
#include "Material.h"

#include "Core/Log.h"

#include "Core/ImGui/ImGuiHelper.h"
#include "Core/Application.h"

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}


namespace Infinit {

	std::shared_ptr<Material> Material::DefaultMaterial;

	MaterialParameterType MaterialParameterTypeFromString(const string& typeString)
	{
		if (typeString == "float") return MaterialParameterType::Float;
		else if (typeString == "vec2") return MaterialParameterType::Float2;
		else if (typeString == "vec3") return MaterialParameterType::Float3;
		else if (typeString == "color3") return MaterialParameterType::Color3;
		else if (typeString == "vec4") return MaterialParameterType::Float4;
		else if (typeString == "color4") return MaterialParameterType::Color4;
		else if (typeString == "bool") return MaterialParameterType::Bool;
		else if (typeString == "int") return MaterialParameterType::Int;
		else if (typeString == "sampler2D") return MaterialParameterType::Texture2D;
		else if (typeString == "samplerCube") return MaterialParameterType::TextureCube;

		return MaterialParameterType::None;
	}

	size_t GetMaterialParameterSize(MaterialParameterType type)
	{
		switch (type)
		{
		case MaterialParameterType::None:
			return 0;
		case MaterialParameterType::Float:
			return sizeof(float);
		case MaterialParameterType::Float2:
			return sizeof(glm::vec2);
		case MaterialParameterType::Float3:
			return sizeof(glm::vec3);
		case MaterialParameterType::Float4:
			return sizeof(glm::vec4);
		case MaterialParameterType::Color3:
			return sizeof(glm::vec3);
		case MaterialParameterType::Color4:
			return sizeof(glm::vec4);
		case MaterialParameterType::Int:
			return sizeof(int);
		case MaterialParameterType::Bool:
			return sizeof(bool);
		case MaterialParameterType::Texture2D:
			return sizeof(std::shared_ptr<Texture2D>);
		case MaterialParameterType::TextureCube:
			return sizeof(std::shared_ptr<TextureCube>);
		}
	}

	bool CheckLuaError(lua_State* L, int err)
	{
		if (err != LUA_OK)
		{
			IN_CORE_ERROR("Lua: {0}", lua_tostring(L, -1));
			lua_pop(L, 1);
			return false;
		}
		return true;
	}

	ParameterPreset::ParameterPreset(const string& name, MaterialParameterType type)
		: m_Name(name), m_Type(type)
	{
	}

	ParameterPreset::~ParameterPreset()
	{
	}

	Parameter* ParameterPreset::CreateParameter()
	{
		return new Parameter(m_Name);
	}

	Material::Material(const string& filePath)
		: Resource(filePath), m_ParamPushMutex()
	{
		Reload(filePath);
		//while (isvalid)
		//{
		//
		//}
	}

	Material::Material(const std::shared_ptr<Shader>& shader)
		: Resource(""), ShaderProgram(shader), m_ParamPushMutex()
	{

	}

	Material::~Material()
	{
		IN_CORE_INFO("Delete Material");
	}

	bool Material::Reload(const string& filePath)
	{
		lua_State* L = luaL_newstate();
		int err = 0;
		
		if (luaL_loadfile(L, filePath.c_str()) || lua_pcall(L, 0, 0, 0))
		{
			IN_CORE_ERROR("Lua: {0}", lua_tostring(L, -1));
			lua_pop(L, 1);
			return false;
		}
		
		

		Application& app = Application::Get();
		err = lua_getglobal(L, "material");
		if (!lua_istable(L, -1))
			IN_CORE_WARN("Material has no Table");
		
		lua_pushstring(L, "name");
		lua_gettable(L, -2);
		
		ChangeName(lua_tostring(L, -1));

		lua_pop(L, 1);


		lua_pushstring(L, "shader");
		lua_gettable(L, -2);
		const string& shaderPath = lua_tostring(L, -1);
		ShaderProgram = app.GetResourceLoader().GetResource<Shader>(shaderPath);
		if (!ShaderProgram)
			app.GetResourceLoader().AddResourceLoadFinishCallback(shaderPath, [this](std::shared_ptr<Resource> shader) { this->ShaderProgram = std::dynamic_pointer_cast<Shader>(shader); });
		lua_pop(L, 1);

		lua_pushstring(L, "textures");
		lua_gettable(L, -2);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			lua_pushstring(L, "name");
			lua_gettable(L, -2);
			string* name = new string(lua_tostring(L, -1));
			lua_pop(L, 1);
			lua_pushstring(L, "path");
			lua_gettable(L, -2);
			const char* path = lua_tostring(L, -1);
			std::shared_ptr<Texture2D> texture = app.GetResourceLoader().GetResource<Texture2D>(path);
			if (!texture)
				app.GetResourceLoader().AddResourceLoadFinishCallback(path, [this, name](std::shared_ptr<Resource> tex) { 
				AddTexture(*name, std::dynamic_pointer_cast<Texture2D>(tex)); 
				delete name;
					});
			else
			{
				AddTexture(*name, texture);
				delete name;
			}
			lua_pop(L, 2);
		}
		
		lua_pop(L, 1);

		lua_pushstring(L, "params");
		lua_gettable(L, -2);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			lua_pushstring(L, "name");
			lua_gettable(L, -2);
			string name = lua_tostring(L, -1);
			lua_pop(L, 1);
			lua_pushstring(L, "type");
			lua_gettable(L, -2);
			const char* type = lua_tostring(L, -1);
			MaterialParameterType shaderType = MaterialParameterTypeFromString(type);

			ParameterPreset preset(name, shaderType);
			
			m_ParameterPresets.push_back(preset);

			switch (shaderType)
			{
			case MaterialParameterType::Bool:   break;
			case MaterialParameterType::Int:  break;
			case MaterialParameterType::Float:  break;
			case MaterialParameterType::Float2: break;
			case MaterialParameterType::Float3:
			case MaterialParameterType::Color3:  break;
			case MaterialParameterType::Float4:
			case MaterialParameterType::Color4: break;
			case MaterialParameterType::Texture2D:
			case MaterialParameterType::TextureCube: break;
			default:
				break;
			}
			//AddTexture(name, std::dynamic_pointer_cast<Texture2D>(app.GetResource(path)));
			lua_pop(L, 2);
		}

		lua_pop(L, 1);
		lua_pushstring(L, "cubemaps");
		lua_gettable(L, -2);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			lua_pushstring(L, "name");
			lua_gettable(L, -2);
			string* name = new string(lua_tostring(L, -1));
			lua_pop(L, 1);
			lua_pushstring(L, "path");
			lua_gettable(L, -2);
			const char* path = lua_tostring(L, -1);
			std::shared_ptr<TextureCube> cubeMap = app.GetResourceLoader().GetResource<TextureCube>(path);
			if (!cubeMap)
			{
				app.GetResourceLoader().AddResourceLoadFinishCallback(path, [this, name](std::shared_ptr<Resource> tex) { 
					this->AddTexture(*name, std::dynamic_pointer_cast<TextureCube>(tex)); 
					delete name;
					});
			}
			else
			{
				AddTexture(*name, cubeMap);
				delete name;
			}
			lua_pop(L, 2);
		}

		lua_close(L);

		return true;
	}

	void MaterialInstance::Bind()
	{
		IN_CORE_ASSERT(m_Shader, "Pls provide a valid shader for the Material");
		m_Shader->Bind();

		for (Parameter* param : m_Params)
		{
			param->Bind(m_Shader);
		}

		m_Shader->UploadUniformBuffer();
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<Texture2D> texture)
	{
		std::lock_guard<std::mutex> lock(m_ParamPushMutex);
		ParameterPreset p(shaderName, MaterialParameterType::Texture2D);
		p.SetDefaultValue<std::shared_ptr<Texture2D>>(texture);
		m_ParameterPresets.push_back(p);
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<TextureCube> texture)
	{
		std::lock_guard<std::mutex> lock(m_ParamPushMutex);
		ParameterPreset p(shaderName, MaterialParameterType::TextureCube);
		p.SetDefaultValue<std::shared_ptr<TextureCube>>(texture);
		m_ParameterPresets.push_back(p);
	}

	//////////////////IMGUI///////////////////////////////
	void ShowAddParameterMenu()
	{
		
		if (ImGui::BeginMenu("Add Parameter"))
		{
			ImGui::MenuItem("Color");
			ImGui::EndMenu();
		}
	}

	void MaterialInstance::DrawImGui()
	{
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Texture"))
			{
				ImGui::OpenPopup("Set Name");
			}
			ImGui::EndPopup();
		}

		bool namePopupOpen = true;
		if (ImGui::BeginPopupModal("Set Name", &namePopupOpen))
		{
			char* name = new char[64];
			ImGui::Button("Close");
			ImGui::InputText("Unknown", name, 64);
			ImGui::EndPopup();
			delete[] name;
		}

		if (m_Shader)
		{
			if (ImGui::TreeNode(("Shader " + m_Shader->GetFilePath()).c_str()))
			{
				std::string buttonName = "Reload##" + m_Shader->GetFilePath();
				if (ImGui::Button(buttonName.c_str()))
					m_Shader->Reload("");
				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::TreeNode("Shader"))
			{
				ImGui::Button("", ImVec2(64, 64));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_NODE"))
					{
						ResourceNode* node = (ResourceNode*)payload->Data;
						if (node->GetType() == ResourceNode::Type::MESH)
						{
							m_Shader = node->GetResource<Shader>();
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::TreePop();
			}
		}

		for (auto& param : m_Params)
		{
			param->DrawImGui();
		}

	}

	MaterialInstance::MaterialInstance(std::shared_ptr<Material> instance)
		: Instance(instance)
	{
		m_Shader = instance->ShaderProgram;
		for (ParameterPreset& param : instance->m_ParameterPresets)
		{
			switch (param.GetType())
			{
			case MaterialParameterType::Bool:			AddParameter(new MaterialParameter<bool>(param.GetName())); break;
			case MaterialParameterType::Int:			AddParameter(new MaterialParameter<int>(param.GetName())); break;
			case MaterialParameterType::Float:			AddParameter(new MaterialParameter<float>(param.GetName())); break;
			case MaterialParameterType::Float2:			AddParameter(new MaterialParameter<glm::vec2>(param.GetName())); break;
			case MaterialParameterType::Float3:
			case MaterialParameterType::Color3:			AddParameter(new MaterialParameter<glm::vec3>(param.GetName())); break;
			case MaterialParameterType::Float4:
			case MaterialParameterType::Color4:			AddParameter(new MaterialParameter<glm::vec4>(param.GetName())); break;
			case MaterialParameterType::Texture2D:		AddParameter(new MaterialParameter<Texture2D>(param.GetName(), param.GetDefaultValue<std::shared_ptr<Texture2D>>())); break;
			case MaterialParameterType::TextureCube:	AddParameter(new MaterialParameter<TextureCube>(param.GetName(), param.GetDefaultValue<std::shared_ptr<TextureCube>>())); break;
			}
			
		}
	}

	MaterialInstance::~MaterialInstance()
	{
		for (Parameter* param : m_Params)
		{
			delete param;
		}
	}

}
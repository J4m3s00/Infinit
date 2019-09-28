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

	void Parameter::BindToShader(std::shared_ptr<Shader> shader)
	{
		m_Buffer = shader->GetUniformBuffer(m_Name);
	}

	Material::Material(const string& filePath)
		: Resource(filePath)
	{
		Reload(filePath);
		//while (isvalid)
		//{
		//
		//}
	}

	Material::Material(const std::shared_ptr<Shader>& shader)
		: Resource(""), ShaderProgram(shader)
	{

	}

	Material::~Material()
	{
		IN_CORE_INFO("Delete Material");
		for (Parameter* p : m_Params)
			delete p;
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
		ShaderProgram = std::dynamic_pointer_cast<Shader>(app.GetResource(lua_tostring(L, -1)));
		lua_pop(L, 1);

		lua_pushstring(L, "textures");
		lua_gettable(L, -2);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			lua_pushstring(L, "name");
			lua_gettable(L, -2);
			const char* name = lua_tostring(L, -1);
			lua_pop(L, 1);
			lua_pushstring(L, "path");
			lua_gettable(L, -2);
			const char* path = lua_tostring(L, -1);
			AddTexture(name, std::dynamic_pointer_cast<Texture2D>(app.GetResource(path)));
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
			const char* name = lua_tostring(L, -1);
			lua_pop(L, 1);
			lua_pushstring(L, "type");
			lua_gettable(L, -2);
			const char* type = lua_tostring(L, -1);
			MaterialParameterType shaderType = MaterialParameterTypeFromString(type);
			switch (shaderType)
			{
			case MaterialParameterType::Bool: AddParameter(new MaterialParameter<bool>(name)); break;
			case MaterialParameterType::Int: AddParameter(new MaterialParameter<int>(name)); break;
			case MaterialParameterType::Float: AddParameter(new MaterialParameter<float>(name)); break;
			case MaterialParameterType::Float2: AddParameter(new MaterialParameter<glm::vec2>(name)); break;
			case MaterialParameterType::Float3: 
			case MaterialParameterType::Color3: AddParameter(new MaterialParameter<glm::vec3>(name)); break;
			case MaterialParameterType::Float4: 
			case MaterialParameterType::Color4: AddParameter(new MaterialParameter<glm::vec4>(name)); break;
			case MaterialParameterType::Texture2D: 
			case MaterialParameterType::TextureCube: AddParameter(new MaterialParameter<int>(name)); break;
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
			const char* name = lua_tostring(L, -1);
			lua_pop(L, 1);
			lua_pushstring(L, "path");
			lua_gettable(L, -2);
			const char* path = lua_tostring(L, -1);
			AddTexture(name, std::dynamic_pointer_cast<TextureCube>(app.GetResource(path)));
			lua_pop(L, 2);
		}

		lua_close(L);

		return true;
	}

	void Material::Bind() const
	{
		IN_CORE_ASSERT(ShaderProgram, "Pls provide a valid shader for the Material");
		ShaderProgram->Bind();
		//for (auto tex : m_Textures)
		//{
		//	uint slot = ShaderProgram->GetResourceSlot(tex.first);
		//	if (slot == -1) IN_CORE_WARN("Could not find texture {0} in shader", tex.first);
		//
		//	ShaderProgram->SetUniform1i(tex.first, slot);
		//	tex.second->Bind(slot);
		//}
		for (const auto& tex : m_Textures)
		{
			uint slot = ShaderProgram->GetResourceSlot(tex.first);
			tex.second->Bind(slot);
		}
		ShaderProgram->UploadUniformBuffer();
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<Texture2D> texture)
	{
		MaterialParameter<int>* param = new MaterialParameter<int>(shaderName);
		AddParameter(param);
		*param->Value = ShaderProgram->GetResourceSlot(shaderName);
		m_Textures[shaderName] = texture;
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<TextureCube> texture)
	{
		MaterialParameter<int>* param = new MaterialParameter<int>(shaderName);
		AddParameter(param);
		*param->Value = ShaderProgram->GetResourceSlot(shaderName);
		m_Textures[shaderName] = texture;
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

	void Material::DrawImGui()
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

		if (ShaderProgram)
		{
			if (ImGui::TreeNode(("Shader " + ShaderProgram->GetFilePath()).c_str()))
			{
				std::string buttonName = "Reload##" + ShaderProgram->GetFilePath();
				if (ImGui::Button(buttonName.c_str()))
					ShaderProgram->Reload("");
				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::TreeNode("Shader"))
			{
				if (ImGui::Button("Load"))
				{
					string filePath = Application::Get().OpenFile(IN_FILE_FILTER_Shader);
					ShaderProgram = std::dynamic_pointer_cast<Shader>(Application::Get().GetResource(filePath));
				}
				ImGui::TreePop();
			}
		}

		for (auto& param : m_Params)
		{
			param->DrawImGui();
		}

	}

}
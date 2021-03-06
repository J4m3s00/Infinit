#include "inpch.h"

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

	TPreset::TPreset(const string& name, MaterialParameterType type)
		: m_Name(name), m_Type(type)
	{
	}

	TPreset::~TPreset()
	{
	}

	template <typename T>
	ParameterPreset<T>::ParameterPreset(const string& name, MaterialParameterType type)
		: TPreset(name, type), m_DefaultValue()
	{
		
	}

	template <typename T>
	ParameterPreset<T>::ParameterPreset(const string& name, MaterialParameterType type, const T& value)
		: TPreset(name, type), m_DefaultValue(value)
	{

	}

	template <typename T>
	void ParameterPreset<T>::SetDefaultValue(const T& value)
	{
		m_DefaultValue = value;
	}

	template <typename T>
	const T& ParameterPreset<T>::GetDefaultValue() const
	{
		return m_DefaultValue;
	}
	
	template <typename T>
	void MaterialParameter<T>::Bind(std::shared_ptr < Shader> shader)
	{
		if (!shader)
		{
			IN_CORE_ERROR("Cant bind MaterialParameter {0} to invalid shader!", GetName());
			return;
		}

		if (!m_Buffer) m_Buffer = shader->GetUniformBuffer(m_Name);
		IN_RENDER_S({
		*((T*)self->m_Buffer) = self->Value;
		});
	}

	Material::Material(const string& name, const string& filepath)
		: Resource(filepath, name)
	{
		if (filepath != "")
			Reload(m_FilePath);
		//while (isvalid)
		//{
		//
		//}
	}

	Material::Material(const std::shared_ptr<Shader>& shader)
		: Resource(""), m_ShaderProgram(shader)
	{

	}

	Material::~Material()
	{
		for (TPreset* preset : m_ParameterPresets)
			delete preset;
	}

	bool Material::Reload(const string& filePath)
	{
#if 0
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
		while (!ShaderProgram && app.GetResourceLoader().ResourceExist(shaderPath, ResourceNode::Type::SHADER))
			ShaderProgram = app.GetResourceLoader().GetResource<Shader>(shaderPath);
		lua_pop(L, 1);

		lua_pushstring(L, "textures");
		lua_gettable(L, -2);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			lua_pushstring(L, "name");
			lua_gettable(L, -2);
			string name(lua_tostring(L, -1));
			lua_pop(L, 1);
			lua_pushstring(L, "path");
			lua_gettable(L, -2);
			const char* path = lua_tostring(L, -1);
			std::shared_ptr<Texture2D> texture;
			while (!texture && app.GetResourceLoader().ResourceExist(path, ResourceNode::Type::TEXTURE))
			{
				texture = app.GetResourceLoader().GetResource<Texture2D>(path);
			}
			if (texture)
				AddTexture(name, texture);
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

			switch (shaderType)
			{
			case MaterialParameterType::Bool: m_ParameterPresets.push_back(new ParameterPreset<bool>(name, shaderType, true)); break;
			case MaterialParameterType::Int: m_ParameterPresets.push_back(new ParameterPreset<int>(name, shaderType)); break;
			case MaterialParameterType::Float: m_ParameterPresets.push_back(new ParameterPreset<float>(name, shaderType)); break;
			case MaterialParameterType::Float2: m_ParameterPresets.push_back(new ParameterPreset<glm::vec2>(name, shaderType)); break;
			case MaterialParameterType::Float3:
			case MaterialParameterType::Color3: m_ParameterPresets.push_back(new ParameterPreset<glm::vec3>(name, shaderType)); break;
			case MaterialParameterType::Float4:
			case MaterialParameterType::Color4: m_ParameterPresets.push_back(new ParameterPreset<glm::vec4>(name, shaderType)); break;
			case MaterialParameterType::Texture2D: m_ParameterPresets.push_back(new ParameterPreset<std::shared_ptr<Texture2D>>(name, shaderType)); break;
			case MaterialParameterType::TextureCube: m_ParameterPresets.push_back(new ParameterPreset<std::shared_ptr<Texture2D>>(name, shaderType)); break;
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
			string name(lua_tostring(L, -1));
			lua_pop(L, 1);
			lua_pushstring(L, "path");
			lua_gettable(L, -2);
			const char* path = lua_tostring(L, -1);
			std::shared_ptr<TextureCube> cubeMap = app.GetResourceLoader().GetResource<TextureCube>(path);
			while (!cubeMap && app.GetResourceLoader().ResourceExist(path, ResourceNode::Type::CUBEMAP))
			{
				cubeMap = app.GetResourceLoader().GetResource<TextureCube>(path);
			}
			if (cubeMap)
				AddTexture(name, cubeMap);
			lua_pop(L, 2);
		}

		lua_close(L);
#endif
		return true;
	}

	void Material::SetShader(std::shared_ptr<Shader> shader)
	{
		m_ShaderProgram = shader;

		///Load params from the shader
		const std::vector<ShaderUniform> shaderUniforms = shader->GetUniforms();

		for (const ShaderUniform& uniform : shaderUniforms) 
		{
			byte* uniformPtr = shader->GetUniformBuffer(uniform.Name);
			switch (uniform.Type)
			{
			case Infinit::ShaderDataType::Float:		m_ParameterPresets.push_back(new ParameterPreset<float>(uniform.Name, MaterialParameterType::Float));	break;
			case Infinit::ShaderDataType::Float2:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec2>(uniform.Name, MaterialParameterType::Float2));	break;
			case Infinit::ShaderDataType::Float3:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec3>(uniform.Name, MaterialParameterType::Float3));	break;
			case Infinit::ShaderDataType::Float4:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec4>(uniform.Name, MaterialParameterType::Float4));	break;
			case Infinit::ShaderDataType::Matrix3:		m_ParameterPresets.push_back(new ParameterPreset<glm::mat3>(uniform.Name, MaterialParameterType::Mat3));	break;
			case Infinit::ShaderDataType::Matrix4:		m_ParameterPresets.push_back(new ParameterPreset<glm::mat4>(uniform.Name, MaterialParameterType::Mat4));	break;
			case Infinit::ShaderDataType::Int:			m_ParameterPresets.push_back(new ParameterPreset<int>(uniform.Name, MaterialParameterType::Float));	break;
			case Infinit::ShaderDataType::Texture2D:	m_ParameterPresets.push_back(new ParameterPreset<std::shared_ptr<Texture2D>>(uniform.Name, MaterialParameterType::Texture2D));	break;
			case Infinit::ShaderDataType::TextureCube:	m_ParameterPresets.push_back(new ParameterPreset<std::shared_ptr<TextureCube>>(uniform.Name, MaterialParameterType::TextureCube));	break;
			case Infinit::ShaderDataType::Int2:			m_ParameterPresets.push_back(new ParameterPreset<glm::ivec2>(uniform.Name, MaterialParameterType::Int2));	break;
			case Infinit::ShaderDataType::Int3:			m_ParameterPresets.push_back(new ParameterPreset<glm::ivec3>(uniform.Name, MaterialParameterType::Int3));	break;
			case Infinit::ShaderDataType::Int4:			m_ParameterPresets.push_back(new ParameterPreset<glm::ivec4>(uniform.Name, MaterialParameterType::Int4));	break;
			case Infinit::ShaderDataType::UInt:			m_ParameterPresets.push_back(new ParameterPreset<uint>(uniform.Name, MaterialParameterType::Uint));	break;
			case Infinit::ShaderDataType::Byte4:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec4>(uniform.Name, MaterialParameterType::Color4));	break;
			case Infinit::ShaderDataType::Bool:			m_ParameterPresets.push_back(new ParameterPreset<bool>(uniform.Name, MaterialParameterType::Bool));	break;
			}
		}
	}

	void MaterialInstance::Bind()
	{
		IN_CORE_ASSERT(!m_Shader.expired(), "Pls provide a valid shader for the Material");
		m_Shader.lock()->Bind();

		for (Parameter* param : m_Params)
		{
			param->Bind(m_Shader.lock());
		}

		m_Shader.lock()->UploadUniformBuffer();
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<Texture2D> texture)
	{
		m_ParameterPresets.push_back(new ParameterPreset< std::shared_ptr<Texture2D>>(shaderName, MaterialParameterType::Texture2D, texture));
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<TextureCube> texture)
	{
		m_ParameterPresets.push_back(new ParameterPreset<std::shared_ptr<TextureCube>>(shaderName, MaterialParameterType::TextureCube, texture));
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

		if (!m_Shader.expired())
		{
			if (ImGui::TreeNode(("Shader " + m_Shader.lock()->GetFilePath()).c_str()))
			{
				std::string buttonName = "Reload##" + m_Shader.lock()->GetFilePath();
				if (ImGui::Button(buttonName.c_str()))
					m_Shader.lock()->Reload("");
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
						if (node->GetType() == ResourceNode::Type::SHADER)
						{
							m_Shader = node->GetResource<Shader>();
							Instance.lock()->SetShader(node->GetResource<Shader>());
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

		if (ImGui::Button("+##SelectShaderResource"))
		{
			ImGui::OpenPopup("Select Shader Resource##ResourceSelect");
		}

		static TPreset* currSelection = NULL;
		bool open = true;
		if (ImGui::BeginPopupModal("Select Shader Resource##ResourceSelect", &open))
		{
			for (auto& param : Instance.lock()->m_ParameterPresets)
			{
				if (ImGui::Selectable(param->GetName().c_str(), (param == currSelection))) {
					currSelection = param;
					AddParamFromPreset(param);
				}
			}
			ImGui::EndPopup();
		}
	}

	MaterialInstance::MaterialInstance(std::weak_ptr<Material> instance)
		: Instance(instance)
	{
		m_Shader = instance.lock()->m_ShaderProgram;
		ReloadPresets();
	}

	MaterialInstance::~MaterialInstance()
	{
		for (Parameter* param : m_Params)
		{
			delete param;
		}
	}

	void MaterialInstance::ReloadPresets()
	{
		if (m_Shader.expired()) return;
		for (TPreset* p : Instance.lock()->m_ParameterPresets)
		{
			AddParamFromPreset(p);
		}
	}

	void MaterialInstance::AddParamFromPreset(TPreset* preset)
	{
		switch (preset->GetType())
		{
		case MaterialParameterType::Bool: { ParameterPreset<bool>* parameter = static_cast<ParameterPreset<bool>*>							(preset); AddParameter(new MaterialParameter<bool>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		case MaterialParameterType::Int: { ParameterPreset<int>* parameter = static_cast<ParameterPreset<int>*>							(preset); AddParameter(new MaterialParameter<int>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		case MaterialParameterType::Float: { ParameterPreset<float>* parameter = static_cast<ParameterPreset<float>*>						(preset); AddParameter(new MaterialParameter<float>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		case MaterialParameterType::Float2: { ParameterPreset<glm::vec2>* parameter = static_cast<ParameterPreset<glm::vec2>*>					(preset); AddParameter(new MaterialParameter<glm::vec2>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		case MaterialParameterType::Float3:
		case MaterialParameterType::Color3: { ParameterPreset<glm::vec3>* parameter = static_cast<ParameterPreset<glm::vec3>*>					(preset); AddParameter(new MaterialParameter<glm::vec3>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		case MaterialParameterType::Float4:
		case MaterialParameterType::Color4: { ParameterPreset<glm::vec4>* parameter = static_cast<ParameterPreset<glm::vec4>*>					(preset); AddParameter(new MaterialParameter<glm::vec4>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		case MaterialParameterType::Texture2D: { ParameterPreset<std::shared_ptr<Texture2D>>* parameter = static_cast<ParameterPreset<std::shared_ptr<Texture2D>>*>	(preset); AddParameter(new MaterialParameter<Texture2D>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		case MaterialParameterType::TextureCube: { ParameterPreset<std::shared_ptr<TextureCube>>* parameter = static_cast<ParameterPreset<std::shared_ptr<TextureCube>>*>	(preset); AddParameter(new MaterialParameter<TextureCube>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		case MaterialParameterType::Mat3: { ParameterPreset<glm::mat3>* parameter = static_cast<ParameterPreset<glm::mat3>*> (preset);  AddParameter(new MaterialParameter<glm::mat3>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		case MaterialParameterType::Mat4: { ParameterPreset<glm::mat4>* parameter = static_cast<ParameterPreset<glm::mat4>*> (preset);  AddParameter(new MaterialParameter<glm::mat4>(parameter->GetName(), parameter->GetDefaultValue())); break; }
		}
	}
}
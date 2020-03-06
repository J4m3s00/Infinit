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

		shader->SetUniformBuffer(m_Name, (byte*) &Value, sizeof(T));
	}

	Material::Material()
		: Resource("", Resource::Type::MATERIAL)
	{
	}

	Material::Material(const string& name, const string& filepath)
		: Resource(filepath, Resource::Type::MATERIAL, name)
	{
		if (filepath != "")
			Reload(m_FilePath.GetValue());
		//while (isvalid)
		//{
		//
		//}
	}

	Material::Material(const std::shared_ptr<Shader>& shader)
		: Resource("", Resource::Type::MATERIAL), m_ShaderProgram(shader)
	{

	}

	Material::~Material()
	{
		for (TPreset* preset : m_ParameterPresets)
			delete preset;
	}

	bool Material::Reload(const string& filePath)
	{
		std::ifstream inFile(filePath);
		json json_object;
		inFile >> json_object;
		this->Deserialize(json_object);
		return true;
	}

	void Material::OnSerialize(json& js_object) const
	{
		Resource::OnSerialize(js_object);
		if (m_ShaderProgram)
		{
			json shaderJson = json::object();
			m_ShaderProgram->Serialize(shaderJson);

			js_object["Shader"] = shaderJson;
		}
	}

	void Material::OnDeserialize(const json& json_object)
	{
		json shader_json = json_object["Shader"];
		if (m_ShaderProgram)
		{
			m_ShaderProgram->Deserialize(shader_json);
		}
		else
		{
			m_ShaderProgram = Shader::Create(shader_json);
		}
		SetShader(m_ShaderProgram);
	}

	void Material::SetShader(std::shared_ptr<Shader> shader)
	{
		m_ShaderProgram = shader;
		for (size_t i = 0; i < m_ParameterPresets.size(); i++) 
			delete m_ParameterPresets[i];
		m_ParameterPresets.clear();

		if (!shader)
		{
			IN_CORE_WARN("Setting invald shader to material");
			return;
		}
		///Load params from the shader
		const std::vector<ShaderUniform> shaderUniforms = shader->GetUniforms();

		for (const ShaderUniform& uniform : shaderUniforms) 
		{
			AddPresetFromType(uniform.Type, uniform.Name);
		}
	}
	
	void Material::AddPresetFromType(ShaderDataType type, const string& name)
	{
		switch (type)
		{
		case Infinit::ShaderDataType::Float:		m_ParameterPresets.push_back(new ParameterPreset<float>(name, MaterialParameterType::Float));	break;
		case Infinit::ShaderDataType::Float2:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec2>(name, MaterialParameterType::Float2));	break;
		case Infinit::ShaderDataType::Float3:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec3>(name, MaterialParameterType::Float3));	break;
		case Infinit::ShaderDataType::Float4:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec4>(name, MaterialParameterType::Float4));	break;
		case Infinit::ShaderDataType::Matrix3:		m_ParameterPresets.push_back(new ParameterPreset<glm::mat3>(name, MaterialParameterType::Mat3));	break;
		case Infinit::ShaderDataType::Matrix4:		m_ParameterPresets.push_back(new ParameterPreset<glm::mat4>(name, MaterialParameterType::Mat4));	break;
		case Infinit::ShaderDataType::Int:			m_ParameterPresets.push_back(new ParameterPreset<int>(name, MaterialParameterType::Float));	break;
		case Infinit::ShaderDataType::Texture2D:	m_ParameterPresets.push_back(new ParameterPreset<std::shared_ptr<Texture2D>>(name, MaterialParameterType::Texture2D));	break;
		case Infinit::ShaderDataType::TextureCube:	m_ParameterPresets.push_back(new ParameterPreset<std::shared_ptr<TextureCube>>(name, MaterialParameterType::TextureCube));	break;
		case Infinit::ShaderDataType::Int2:			m_ParameterPresets.push_back(new ParameterPreset<glm::ivec2>(name, MaterialParameterType::Int2));	break;
		case Infinit::ShaderDataType::Int3:			m_ParameterPresets.push_back(new ParameterPreset<glm::ivec3>(name, MaterialParameterType::Int3));	break;
		case Infinit::ShaderDataType::Int4:			m_ParameterPresets.push_back(new ParameterPreset<glm::ivec4>(name, MaterialParameterType::Int4));	break;
		case Infinit::ShaderDataType::UInt:			m_ParameterPresets.push_back(new ParameterPreset<uint>(name, MaterialParameterType::Uint));	break;
		case Infinit::ShaderDataType::Byte4:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec4>(name, MaterialParameterType::Color4));	break;
		case Infinit::ShaderDataType::Bool:			m_ParameterPresets.push_back(new ParameterPreset<bool>(name, MaterialParameterType::Bool));	break;
		}
	}

	void Material::AddPresetFromType(MaterialParameterType type, const string& name)
	{
		switch (type)
		{
		case MaterialParameterType::Float:		m_ParameterPresets.push_back(new ParameterPreset<float>(name, MaterialParameterType::Float));	break;
		case MaterialParameterType::Float2:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec2>(name, MaterialParameterType::Float2));	break;
		case MaterialParameterType::Float3:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec3>(name, MaterialParameterType::Float3));	break;
		case MaterialParameterType::Float4:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec4>(name, MaterialParameterType::Float4));	break;
		case MaterialParameterType::Color3:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec3>(name, MaterialParameterType::Color3));	break;
		case MaterialParameterType::Color4:		m_ParameterPresets.push_back(new ParameterPreset<glm::vec4>(name, MaterialParameterType::Color4));	break;
		case MaterialParameterType::Mat3:		m_ParameterPresets.push_back(new ParameterPreset<glm::mat3>(name, MaterialParameterType::Mat3));	break;
		case MaterialParameterType::Mat4:		m_ParameterPresets.push_back(new ParameterPreset<glm::mat4>(name, MaterialParameterType::Mat4));	break;
		case MaterialParameterType::Int:			m_ParameterPresets.push_back(new ParameterPreset<int>(name, MaterialParameterType::Float));	break;
		case MaterialParameterType::Texture2D:	m_ParameterPresets.push_back(new ParameterPreset<std::shared_ptr<Texture2D>>(name, MaterialParameterType::Texture2D));	break;
		case MaterialParameterType::TextureCube:	m_ParameterPresets.push_back(new ParameterPreset<std::shared_ptr<TextureCube>>(name, MaterialParameterType::TextureCube));	break;
		case MaterialParameterType::Int2:			m_ParameterPresets.push_back(new ParameterPreset<glm::ivec2>(name, MaterialParameterType::Int2));	break;
		case MaterialParameterType::Int3:			m_ParameterPresets.push_back(new ParameterPreset<glm::ivec3>(name, MaterialParameterType::Int3));	break;
		case MaterialParameterType::Int4:			m_ParameterPresets.push_back(new ParameterPreset<glm::ivec4>(name, MaterialParameterType::Int4));	break;
		case MaterialParameterType::Uint:			m_ParameterPresets.push_back(new ParameterPreset<uint>(name, MaterialParameterType::Uint));	break;
		case MaterialParameterType::Bool:			m_ParameterPresets.push_back(new ParameterPreset<bool>(name, MaterialParameterType::Bool));	break;
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
		bool showFileDialog = true;
		if (ImGui::Button("Save##SaveMaterial"))
		{
			if (!Application::Get().GetResourceLoader().ResourceExist(Instance.lock()->GetFilePath(), Resource::MATERIAL))
			{
				ImGui::OpenPopup("Open File##FileDialog");
			}
			else
			{

				std::ofstream o(Instance.lock()->GetFilePath());
				json js = json::object();
				Instance.lock()->Serialize(js);
				o << std::setw(4) << js << std::endl;
			}
		}
		if (!Instance.expired())
		{
			if (!Application::Get().GetResourceLoader().ResourceExist(Instance.lock()->GetFilePath(), Resource::MATERIAL))
			{
				ResourceNode* node;
				if (Application::Get().GetResourceLoader().ShowFileDialog(Resource::Type::FOLDER, &node, &showFileDialog))
				{
					if (node)
					{
						string materialName = node->GetFullPath().c_str();
						materialName.append("/");
						materialName.append(Instance.lock()->GetName().c_str());
						materialName.append(".inm");

						Instance.lock()->m_FilePath.SetValue(materialName);

						std::ofstream o(materialName);
						json js = json::object();
						Instance.lock()->Serialize(js);
						o << std::setw(4) << js << std::endl;
					}
					else { IN_CORE_ERROR("Returned an invalid node from the file dialog!"); }
				}
			}
		}
		
		if (!m_Shader.expired())
		{
			if (ImGui::TreeNode(("Shader " + m_Shader.lock()->GetFilePath()).c_str()))
			{
				std::string buttonName = "Reload##" + m_Shader.lock()->GetFilePath();
				if (ImGui::Button(buttonName.c_str()))
				{
					std::ifstream i(m_Shader.lock()->GetFilePath());
					json json_object;
					i >> json_object;
					m_Shader.lock()->Deserialize(json_object);

					Instance.lock()->SetShader(Instance.lock()->m_ShaderProgram);
				}
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
						if (node->GetResource<Resource>() && node->GetResource<Resource>().get()->GetType())
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

	void MaterialInstance::UpdateShader(bool reloadParams)
	{
		if (Instance.expired()) return;
		m_Shader = Instance.lock()->m_ShaderProgram;
		if (reloadParams)
		{
			ReloadPresets();
		}
	}

	void MaterialInstance::AddParameter(const string& name)
	{
		for (TPreset* p : Instance.lock()->m_ParameterPresets)
		{
			if (p->GetName() == name)
			{
				AddParamFromPreset(p);
				return;
			}
		}
		IN_CORE_WARN("Could not find parameter \"{0}\" in shader \"{1}\"!", name, m_Shader.lock()->GetName());
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
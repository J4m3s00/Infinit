#pragma once

namespace Infinit {

	class Parameter;

	enum class MaterialParameterType
	{
		None = 0, Float, Float2, Float3, Float4, Color3, Color4, Int, Bool, Texture2D, TextureCube, Mat4, Mat3, Int2, Int3, Int4, Uint
	};

	class TPreset : public Serializable
	{
	public:
		TPreset(const string& name, MaterialParameterType type);
		~TPreset();

		MaterialParameterType GetType() const { return m_Type; }
		const string& GetName() const { return m_Name; }

		virtual json Serialize() const override;
		virtual void Deserialize(const json& json_object) override;
	private:
		MaterialParameterType m_Type;
		string m_Name;
	};

	template <typename T>
	class ParameterPreset : public TPreset
	{
	public:
		ParameterPreset(const string& name, MaterialParameterType type);
		ParameterPreset(const string& name, MaterialParameterType type, const T& value);

		virtual json Serialize() const override;
		virtual void Deserialize(const json& json_object) override;

		void SetDefaultValue(const T& value);
		const T& GetDefaultValue() const;
	private:
		T m_DefaultValue;
	};

	class Parameter
	{
	public:
		Parameter(const string& name)
			: m_Name(name), m_Buffer(NULL)
		{

		}

		virtual void Bind(std::shared_ptr<Shader> shader) { if (!m_Buffer) m_Buffer = shader->GetUniformBuffer(m_Name); }
		virtual void DrawImGui() {}
		const string& GetName() const { return m_Name; }
		void* GetBufferPointer() { return m_Buffer; }
	protected:
		byte* m_Buffer;
		string m_Name;
	};

	template <typename T>
	class MaterialParameter : public Parameter
	{
	public:
		MaterialParameter(const string& name)
			: Parameter(name), Value()
		{
		}

		MaterialParameter(const string& name, const T& value)
			: Parameter(name), Value(value)
		{
			
		}

		virtual void Bind(std::shared_ptr<Shader> shader);


		virtual void DrawImGui() override
		{
			ImGuiProperty(GetName(), (T*)&Value);
		}
	public:
		T Value;
	};

	template <>
	class MaterialParameter<Texture2D> : public Parameter
	{
	public:
		MaterialParameter(const string& name, std::shared_ptr<Texture2D> texture)
			: Parameter(name), Slot(-1), Texture(texture)
		{

		}

		virtual void Bind(std::shared_ptr<Shader> shader) override
		{
			if (!shader)
			{
				IN_CORE_ERROR("Cant bind MaterialParameter {0} to invalid shader!", GetName());
				return;
			}
			if (Slot == -1)
				Slot = shader->GetResourceSlot(GetName());

			shader->SetUniformBuffer(m_Name, (byte*) &Slot, sizeof(int));
			if (this->Texture)
				this->Texture->Bind(Slot);
		}

		virtual void DrawImGui() override
		{
			ImGuiProperty(GetName(), &Slot);
			Texture = ImGuiPropertyTex2D("Texture:", Texture);
		}
	public:
		int Slot;
		std::shared_ptr<Texture2D> Texture;
	};

	template <>
	class MaterialParameter<TextureCube> : public Parameter
	{
	public:
		MaterialParameter(const string& name, std::shared_ptr<TextureCube> texture)
			: Parameter(name), Slot(-1), Texture(texture)
		{

		}

		virtual void Bind(std::shared_ptr<Shader> shader) override
		{
			if (!shader)
			{
				IN_CORE_ERROR("Cant bind MaterialParameter {0} to invalid shader!", GetName());
				return;
			}
			if (Slot == -1) Slot = shader->GetResourceSlot(GetName());
			shader->SetUniformBuffer(m_Name, (byte*)&Slot, sizeof(int));
			if (this->Texture)
				this->Texture->Bind(Slot);
		}

		virtual void DrawImGui() override
		{
			ImGuiProperty(GetName(), &Slot);
			//Should be better
			Texture = ImGuiPropertyTexCube("Texture:", Texture);
		}
	public:
		int Slot;
		std::shared_ptr<TextureCube> Texture;
	};

	class Material : public Resource, Serializable
	{
		friend class MaterialInstance;
	public:
		Material(const string& name, const string& filepath = "");
		Material(const std::shared_ptr<Shader>& shader);
		virtual ~Material();

		virtual json Serialize() const override;
		virtual void Deserialize(const json& json_object) override;

		void SetShader(std::shared_ptr<Shader> shader);

		virtual bool Reload(const string& filePath) override;

		void AddTexture(const string& shaderName, std::shared_ptr<Texture2D> texture);
		void AddTexture(const string& shaderName, std::shared_ptr<TextureCube> texture);
	private:
		void AddPresetFromType(ShaderDataType type, const string& name);
		void AddPresetFromType(MaterialParameterType type, const string& name);
	private:
		std::shared_ptr<Shader> m_ShaderProgram;
		std::vector<TPreset*> m_ParameterPresets;
	public:
		static std::shared_ptr<Material> DefaultMaterial;
	};

	class MaterialInstance
	{
	public:
		MaterialInstance(std::weak_ptr<Material> instance);
		~MaterialInstance();

		std::shared_ptr<Shader> GetShaderProgram() { return m_Shader.lock(); }

		void AddParameter(const string& name);

		template <typename T>
		void AddParameter(MaterialParameter<T>* param)
		{
			m_Params.push_back(param);
		}

		template <typename T>
		MaterialParameter<T>* GetParameter(const string& name) const
		{
			for (auto& param : m_Params)
				if (param->GetName() == name)
					return (MaterialParameter<T>*) param;
			IN_CORE_WARN("Could not find material parameter \"{0}\"", name);
			return nullptr;
		}

		void UpdateShader(bool reloadParams = false);

		void Bind();
		void DrawImGui();
	private:
		void ReloadPresets();
		void AddParamFromPreset(TPreset* preset);
	public:
		std::weak_ptr<Material> Instance;
	private:
		std::weak_ptr<Shader> m_Shader;
		std::vector<Parameter*> m_Params;
	};

}
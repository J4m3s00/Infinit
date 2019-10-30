#pragma once

#include "inpch.h"
#include "Core/Core.h"
#include "Core/Resource.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Buffer.h"
#include "Core/ImGui/ImGuiHelper.h"

namespace Infinit {

	enum class MaterialParameterType
	{
		None = 0, Float, Float2, Float3, Float4, Color3, Color4, Int, Bool, Texture2D, TextureCube
	};

	class Parameter
	{
	public:
		Parameter(const string& name)
			: m_Name(name), m_Buffer(NULL)
		{

		}
		Parameter(const Parameter& copy)
			: m_Name(copy.GetName())
		{
			m_Buffer = copy.m_Buffer;
		}

		virtual Parameter* Copy()
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

		virtual void Bind(std::shared_ptr<Shader> shader)
		{
			if (!shader)
			{
				IN_CORE_ERROR("Cant bind MaterialParameter {0} to invalid shader!", GetName());
				return;
			}


			if (!m_Buffer) m_Buffer = shader->GetUniformBuffer(m_Name);

			* ((T*)m_Buffer) = Value;
		}


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

			if (!m_Buffer) m_Buffer = shader->GetUniformBuffer(m_Name);
			if (Slot == -1) Slot = shader->GetResourceSlot(GetName());

			*(int*)m_Buffer = Slot;
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

			if (!m_Buffer) m_Buffer = shader->GetUniformBuffer(m_Name);
			if (Slot == -1) Slot = shader->GetResourceSlot(GetName());


			*(int*)m_Buffer = Slot;
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

	class Material : public Resource
	{
		friend class MaterialInstance;
	public:
		Material(const string& filePath);
		Material(const std::shared_ptr<Shader>& shader);
		virtual ~Material();

		virtual bool Reload(const string& filePath) override;


		void AddTexture(const string& shaderName, std::shared_ptr<Texture2D> texture);
		void AddTexture(const string& shaderName, std::shared_ptr<TextureCube> texture);

		void ResolveMaterialParameters();

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
			return nullptr;
		}

	public:
		std::shared_ptr<Shader> ShaderProgram;
	private:
		std::mutex m_ParamPushMutex;
		std::vector<Parameter*> m_Params;
	public:
		static std::shared_ptr<Material> DefaultMaterial;
	};

	class MaterialInstance
	{
	public:
		MaterialInstance(std::shared_ptr<Material> instance);
		~MaterialInstance();

		std::shared_ptr<Shader> GetShaderProgram() { return m_Shader; }

		void Bind();
		void DrawImGui();
	public:
		std::shared_ptr<Material> Instance;
	private:
		std::shared_ptr<Shader> m_Shader;
		std::vector<Parameter*> m_Params;
	};

}
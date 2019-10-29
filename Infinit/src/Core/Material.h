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
		virtual void Bind() {}
		virtual void DrawImGui() {}
		virtual void BindToShader(std::shared_ptr<Shader> shader);
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
			: Parameter(name), Value(NULL)
		{
		}

		virtual void BindToShader(std::shared_ptr<Shader> shader)
		{
			m_Buffer = shader->GetUniformBuffer(m_Name);
			Value = ((T*)m_Buffer);
		}

		virtual void DrawImGui() override
		{
			ImGuiProperty(GetName(), (T*)Value);
		}
	public:
		T* Value;
	};

	template <>
	class MaterialParameter<Texture2D> : public Parameter
	{
	public:
		MaterialParameter(const string& name, std::shared_ptr<Texture2D> texture)
			: Parameter(name), Slot(0), Texture(texture)
		{

		}

		virtual void Bind() override
		{
			if (this->Texture)
				this->Texture->Bind(*Slot);
		}

		virtual void BindToShader(std::shared_ptr<Shader> shader) override
		{
			if (!shader)
			{
				IN_CORE_ERROR("Cant bind MaterialParameter {0} to invalid shader!", GetName());
				return;
			}
			m_Buffer = shader->GetUniformBuffer(m_Name);
			Slot = ((int*)m_Buffer);
			if (Texture)
				*Slot = shader->GetResourceSlot(GetName());
		}

		virtual void DrawImGui() override
		{
			ImGuiProperty(GetName(), Slot);
			Texture = ImGuiPropertyTex2D("Texture:", Texture);
		}
	public:
		int* Slot;
		std::shared_ptr<Texture2D> Texture;
	};

	template <>
	class MaterialParameter<TextureCube> : public Parameter
	{
	public:
		MaterialParameter(const string& name, std::shared_ptr<TextureCube> texture)
			: Parameter(name), Slot(0), Texture(texture)
		{

		}

		virtual void Bind() override
		{
			if (this->Texture)
				this->Texture->Bind(*Slot);
		}

		virtual void BindToShader(std::shared_ptr<Shader> shader) override
		{
			if (!shader)
			{
				IN_CORE_ERROR("Cant bind MaterialParameter {0} to invalid shader!", GetName());
				return;
			}
			m_Buffer = shader->GetUniformBuffer(m_Name);
			Slot = ((int*)m_Buffer);
			if (Texture)
				*Slot = shader->GetResourceSlot(GetName());
		}

		virtual void DrawImGui() override
		{
			ImGuiProperty(GetName(), Slot);
			//Should be better
			Texture = ImGuiPropertyTexCube("Texture:", Texture);
		}
	public:
		int* Slot;
		std::shared_ptr<TextureCube> Texture;
	};

	class Material : public Resource
	{
	public:
		Material(const string& filePath);
		Material(const std::shared_ptr<Shader>& shader);
		virtual ~Material();

		virtual bool Reload(const string& filePath) override;

		void Bind();

		void AddTexture(const string& shaderName, std::shared_ptr<Texture2D> texture);
		void AddTexture(const string& shaderName, std::shared_ptr<TextureCube> texture);

		void ResolveMaterialParameters();

		template <typename T>
		void AddParameter(MaterialParameter<T>* param)
		{
			m_Params.push_back(param);
			m_Dirty = true;
		}

		template <typename T>
		MaterialParameter<T>* GetParameter(const string& name) const
		{
			for (auto& param : m_Params)
				if (param->GetName() == name)
					return (MaterialParameter<T>*) param;
			return nullptr;
		}

		void DrawImGui();
	public:
		std::shared_ptr<Shader> ShaderProgram;
	private:
		std::mutex m_ParamPushMutex;
		std::unordered_map<string, std::shared_ptr<Texture>> m_Textures;
		std::vector<Parameter*> m_Params;
		mutable bool m_Dirty;
	public:
		static std::shared_ptr<Material> DefaultMaterial;
	};

}
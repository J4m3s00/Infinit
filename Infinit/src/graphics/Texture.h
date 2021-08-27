#pragma once

namespace Infinit {

	enum class TextureFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2
	};

	class Texture : public Resource
	{
	public:
		Texture(const string& filePath, Resource::Type type, const string& name = "Unknown") : Resource(filePath, type, name) {}
		virtual ~Texture() {  }

		virtual void Bind(uint slot) const = 0;
		
		virtual TextureFormat GetFormat() const = 0;
		virtual uint GetWidth() const = 0;
		virtual uint GetHeight() const = 0;
		virtual uint GetRendererID() const = 0;
		

		virtual bool Reload(const string& filePath) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const string& filePath, const string& name = "Unknown") : Texture(filePath, Resource::Type::TEXTURE, name) {}

		static std::shared_ptr<Texture2D> Create(const string& path, bool srgb = false);
		static std::shared_ptr<Texture2D> Create(TextureFormat format, uint width, uint height);
		static std::shared_ptr<Texture2D> Create(const json& json_object);
	};

	class TextureCube : public Texture
	{
	public:
		TextureCube(const string& filePath, const string& name = "Unknown") : Texture(filePath, Resource::Type::CUBEMAP, name) {}

		static std::shared_ptr<TextureCube> Create(const string& path, bool srgb = false);
		static std::shared_ptr<TextureCube> Create(const json& json_object);
	};

}

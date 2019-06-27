#pragma once

#include "Core/Core.h"

namespace Infinit {

	enum class TextureFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2
	};

	class Texture
	{
	public:
		virtual ~Texture() {  }

		virtual void Bind(uint slot) const = 0;
		
		virtual TextureFormat GetFormat() const = 0;
		virtual uint GetWidth() const = 0;
		virtual uint GetHeight() const = 0;

		virtual const string& GetPath() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(const string& path, bool srgb = false);
		static std::shared_ptr<Texture2D> Create(TextureFormat format, uint width, uint height);
	};

	class TextureCube : public Texture
	{
	public:
		static std::shared_ptr<TextureCube> Create(const string& path, bool srgb = false);
	};

}
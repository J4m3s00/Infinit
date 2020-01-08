#include "inpch.h"

namespace Infinit {

	std::shared_ptr<Texture2D> Texture2D::Create(const string& path, bool srgb)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No Renderer selected!");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL: return std::shared_ptr<OpenGLTexture2D>(new OpenGLTexture2D(path, srgb));
		}
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(TextureFormat format, uint width, uint height)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No Renderer selected!");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL: return std::shared_ptr<OpenGLTexture2D>(new OpenGLTexture2D(format, width, height));
		}
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(const json& json_object)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No Renderer selected!");

		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL: { std::shared_ptr<OpenGLTexture2D> result = std::make_shared<OpenGLTexture2D>(); result->Deserialize(json_object); return result; }
		}
		return nullptr;
	}

	std::shared_ptr<TextureCube> TextureCube::Create(const string& path, bool srgb)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No Renderer selected!");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL: return std::shared_ptr<OpenGLTextureCube>(new OpenGLTextureCube(path, srgb));
		}
		return nullptr;
	}

	std::shared_ptr<TextureCube> TextureCube::Create(const json& json_object)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No Renderer selected!");
		
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL: { std::shared_ptr<OpenGLTextureCube> result = std::make_shared<OpenGLTextureCube>(); result->Deserialize(json_object); return result; }
		}
		return nullptr;
	}

}
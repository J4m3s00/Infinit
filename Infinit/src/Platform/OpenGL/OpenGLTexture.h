#pragma once

namespace Infinit {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const string& path, bool srgb);
		OpenGLTexture2D(TextureFormat format, uint width, uint height);

		virtual ~OpenGLTexture2D();

		virtual uint GetRendererID() const { return m_RendererID; }
		virtual void Bind(uint slot) const override;

		virtual TextureFormat GetFormat() const override { return m_Format; }
		virtual uint GetHeight() const override { return m_Height; }
		virtual uint GetWidth() const override { return m_Width; }

		virtual bool Reload(const string& filepath);

		virtual const string& GetPath() const override { return m_FilePath; }
	private:
		uint m_RendererID;
		TextureFormat m_Format;
		uint m_Width;
		uint m_Height;
		string m_FilePath;
		byte* m_ImageData;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(const string& path, bool srgb = false);

		virtual ~OpenGLTextureCube();

		virtual uint GetRendererID() const { return m_RendererID; }
		virtual void Bind(uint slot) const override;

		virtual TextureFormat GetFormat() const override { return m_Format; }
		virtual uint GetHeight() const override { return m_Height; }
		virtual uint GetWidth() const override { return m_Width; }

		virtual bool Reload(const string& filePath);

		virtual const string& GetPath() const override { return m_FilePath; }
	private:
		uint m_RendererID;
		TextureFormat m_Format;
		uint m_Width;
		uint m_Height;
		string m_FilePath;
		byte* m_ImageData;
	};

}

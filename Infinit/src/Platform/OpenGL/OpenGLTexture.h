#pragma once

namespace Infinit {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D();
		OpenGLTexture2D(const string& path, bool srgb);
		OpenGLTexture2D(TextureFormat format, uint width, uint height);

		virtual ~OpenGLTexture2D();

		virtual uint GetRendererID() const { return m_RendererID; }
		virtual void Bind(uint slot) const override;

		virtual TextureFormat GetFormat() const override { return m_Format; }
		virtual uint GetHeight() const override { return (uint) m_Height; }
		virtual uint GetWidth() const override { return (uint) m_Width; }

		virtual bool Reload(const string& filepath);

		virtual void ImGuiDraw() override;

		virtual json Serialize() const override;
		virtual void Deserialize(const json& json_object) override;
	private:
		void CreateTexture(byte* data);
	private:
		string m_TexturePath;
		uint m_RendererID;
		TextureFormat m_Format;
		int m_Width;
		int m_Height;
		int m_Channels;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube();
		OpenGLTextureCube(const string& path, bool srgb = false);

		virtual ~OpenGLTextureCube();

		virtual uint GetRendererID() const { return m_RendererID; }
		virtual void Bind(uint slot) const override;

		virtual TextureFormat GetFormat() const override { return m_Format; }
		virtual uint GetHeight() const override { return (uint) m_Height; }
		virtual uint GetWidth() const override { return (uint) m_Width; }

		virtual bool Reload(const string& filePath);

		virtual json Serialize() const override;
		virtual void Deserialize(const json& json_object) override;
	private:
		void CreateCubeMap(byte* data);
	private:
		string m_TexturePath;
		uint m_RendererID;
		TextureFormat m_Format;
		int m_Width;
		int m_Height;
		int m_Channels;
		std::vector<byte> m_ImageData;
	};

}

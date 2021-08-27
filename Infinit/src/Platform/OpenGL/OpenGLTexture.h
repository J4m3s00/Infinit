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

		virtual TextureFormat GetFormat() const override { return (TextureFormat) m_Format.GetValue(); }
		virtual uint GetHeight() const override { return (uint) m_Height.GetValue(); }
		virtual uint GetWidth() const override { return (uint) m_Width.GetValue(); }

		virtual bool Reload(const string& filepath);

		virtual void ImGuiDraw() override;

		virtual void OnDeserialize(const json& json_object) override;
	private:
		void CreateTexture(byte* data);
	private:
		ValueProperty<string> m_TexturePath;
		uint m_RendererID;
		ValueProperty<int> m_Format;
		ValueProperty<int> m_Width;
		ValueProperty<int> m_Height;
		ValueProperty<int> m_Channels;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube();
		OpenGLTextureCube(const string& path, bool srgb = false);

		virtual ~OpenGLTextureCube();

		virtual uint GetRendererID() const { return m_RendererID; }
		virtual void Bind(uint slot) const override;

		virtual TextureFormat GetFormat() const override { return (TextureFormat) m_Format.GetValue(); }
		virtual uint GetHeight() const override { return (uint) m_Height.GetValue(); }
		virtual uint GetWidth() const override { return (uint) m_Width.GetValue(); }

		virtual bool Reload(const string& filePath);

		virtual void OnDeserialize(const json& json_object) override;
	private:
		void CreateCubeMap(byte* data);
	private:
		ValueProperty<string> m_TexturePath;
		uint m_RendererID;
		ValueProperty<int> m_Format;
		ValueProperty<int> m_Width;
		ValueProperty<int> m_Height;
		ValueProperty<int> m_Channels;
	};

}

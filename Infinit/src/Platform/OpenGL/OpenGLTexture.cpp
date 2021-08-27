#include "inpch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Infinit {

	static GLenum InfinitToOpenGLTextureFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB:     return GL_RGB;
		case TextureFormat::RGBA:    return GL_RGBA;
		}
		return 0;
	}

	static int CalculateMipMapCount(int width, int height)
	{
		int levels = 1;
		while ((width | height) >> levels) {
			levels++;
		}
		return levels;
	}

	//Remove srgb??

	OpenGLTexture2D::OpenGLTexture2D()
		: Texture2D(""), m_Format("Format", this, (int) TextureFormat::None),
		m_Width("Width", this, 0),
		m_Height("Height", this, 0),
		m_RendererID(0),
		m_Channels("Channels", this, 0),
		m_TexturePath("TexturePath", this, "")
	{

	}

	OpenGLTexture2D::OpenGLTexture2D(const string& path, bool srgb)
		: Texture2D(path), 
		m_Format("Format", this, (int) TextureFormat::None),
		m_Width("Width", this, 0),
		m_Height("Height", this, 0),
		m_RendererID(0),
		m_Channels("Channels", this, 0),
		m_TexturePath("TexturePath", this, "")
	{
		Reload(path);
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint width, uint height)
		: Texture2D(""), 
		m_Format("Format", this, (int)format),
		m_Width("Width", this, width), 
		m_Height("Height", this, height), 
		m_RendererID(0), 
		m_Channels("Channels", this, 0),
		m_TexturePath("TexturePath", this, "")
	{
		IN_RENDER_S({
			glGenTextures(1, &self->m_RendererID);
			glBindTexture(GL_TEXTURE_2D, self->m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, InfinitToOpenGLTextureFormat(self->GetFormat()), self->GetWidth(), self->GetHeight(), 0, InfinitToOpenGLTextureFormat(TextureFormat::RGBA), GL_UNSIGNED_BYTE, NULL);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		})
	}


	void OpenGLTexture2D::OnDeserialize(const json& json_object)
	{
		Texture::OnDeserialize(json_object);
		int width;
		int height;
		int channels;
		byte* data = stbi_load(m_TexturePath.GetValue().c_str(), &width, &height, &channels, STBI_rgb_alpha);
		m_Width.SetValue(width);
		m_Height.SetValue(height);
		m_Channels.SetValue(channels);

		m_Format.SetValue((int)(m_Channels.GetValue() == 3 ? TextureFormat::RGB : TextureFormat::RGBA));

		CreateTexture(data);
	}

	void OpenGLTexture2D::CreateTexture(byte* data)
	{
		IN_RENDER_S1(data, {

					glGenTextures(1, &self->m_RendererID);
					glBindTexture(GL_TEXTURE_2D, self->m_RendererID);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					glTexImage2D(GL_TEXTURE_2D, 0, InfinitToOpenGLTextureFormat(self->GetFormat()), self->GetWidth(), self->GetHeight(), 0, InfinitToOpenGLTextureFormat(TextureFormat::RGBA), GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);

					glBindTexture(GL_TEXTURE_2D, 0);
					stbi_image_free(data);
			})
	}

	bool OpenGLTexture2D::Reload(const string& filePath)
	{
		if (filePath != "")
			m_FilePath.SetValue(filePath);
		std::ifstream inFile(GetFilePath());
		json json_object;


		size_t dotPos = GetFilePath().find_last_of(".");

		string fileEnding = GetFilePath().substr(dotPos + 1, GetFilePath().size());

		if (fileEnding == "inr")
		{
			inFile >> json_object;
		}
		
		if (!json_object.is_null())
		{
			Deserialize(json_object);
		}
		else
		{
			stbi_set_flip_vertically_on_load(false);
			int width;
			int height;
			int channels;
			byte* data = stbi_load(GetFilePath().c_str(), &width, &height, &channels, STBI_rgb_alpha);
			m_Width.SetValue(width);
			m_Height.SetValue(height);
			m_Channels.SetValue(channels);
			m_TexturePath.SetValue(GetFilePath());

			m_Format.SetValue((int) (m_Channels.GetValue() == 3 ? TextureFormat::RGB : TextureFormat::RGBA));
			CreateTexture(data);
		}
		return true;
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		IN_CORE_INFO("OpenGL Texture detroyed!");
		IN_RENDER_S({
				glDeleteTextures(1, &self->m_RendererID);
			})
		
	}

	void OpenGLTexture2D::Bind(uint slot) const
	{
		IN_RENDER_S1(slot, {
				glBindTextureUnit(slot, self->m_RendererID);
			})

	}

	void OpenGLTexture2D::ImGuiDraw()
	{
		Texture::ImGuiDraw();
		auto viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)m_RendererID, viewportSize);
	}

//REMOVE srgb??

	OpenGLTextureCube::OpenGLTextureCube()
		: TextureCube(""), m_Width("Width", this, 0), 
		m_Height("Height", this, 0),
		m_Format("Format", this, (int) TextureFormat::None),
		m_TexturePath("TexturePath", this, ""), 
		m_Channels("Channels", this, 0)
	{

	}

	OpenGLTextureCube::OpenGLTextureCube(const string& path, bool srgb)
		: TextureCube(path), 
		m_Width("Width", this, 0),
		m_Height("Height", this, 0),
		m_Format("Format", this, (int)TextureFormat::None),
		m_TexturePath("TexturePath", this, ""), 
		m_Channels("Channels", this, 0)
	{
		Reload(path);
	}

	bool OpenGLTextureCube::Reload(const string& filePath)
	{
		std::ifstream inFile(filePath);
		json json_object;
		if (!json_object.is_null())
		{
			Deserialize(json_object);
		}
		else
		{
			int width, height, channels;
				stbi_set_flip_vertically_on_load(false);
				stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb);


				m_Width.SetValue(width);
				m_Height.SetValue(height);
				m_Format.SetValue((int)TextureFormat::RGB);
				m_TexturePath.SetValue(filePath);
				m_FilePath.SetValue(filePath);

				CreateCubeMap(data);

				
			}
		return true;
	}

	void OpenGLTextureCube::CreateCubeMap(byte* data)
	{
		unsigned int faceWidth = GetWidth() / 4;
		unsigned int faceHeight = GetHeight() / 3;
		IN_CORE_ASSERT(faceWidth == faceHeight, "Non-square faces!");

		std::array<unsigned char*, 6> faces;
		for (size_t i = 0; i < faces.size(); i++)
			faces[i] = new unsigned char[faceWidth * faceHeight * 3u]; // 3 BPP

		int faceIndex = 0;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + i * faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = data[(xOffset + yOffset * GetWidth()) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = data[(xOffset + yOffset * GetWidth()) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = data[(xOffset + yOffset * GetWidth()) * 3 + 2];
				}
			}
			faceIndex++;
		}

		for (size_t i = 0; i < 3; i++)
		{
			// Skip the middle one
			if (i == 1)
				continue;

			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + i * faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = data[(xOffset + yOffset * GetWidth()) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = data[(xOffset + yOffset * GetWidth()) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = data[(xOffset + yOffset * GetWidth()) * 3 + 2];
				}
			}
			faceIndex++;
		}

		IN_RENDER_S2(faces, data, {

			unsigned int faceWidth = self->GetWidth() / 4;
			unsigned int faceHeight = self->GetHeight() / 3;

			glGenTextures(1, &self->m_RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, self->m_RendererID);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			auto format = InfinitToOpenGLTextureFormat(self->GetFormat());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, faceWidth, faceHeight, 0, InfinitToOpenGLTextureFormat(TextureFormat::RGB), GL_UNSIGNED_BYTE, faces[2]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, faceWidth, faceHeight, 0, InfinitToOpenGLTextureFormat(TextureFormat::RGB), GL_UNSIGNED_BYTE, faces[0]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, faceWidth, faceHeight, 0, InfinitToOpenGLTextureFormat(TextureFormat::RGB), GL_UNSIGNED_BYTE, faces[4]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, faceWidth, faceHeight, 0, InfinitToOpenGLTextureFormat(TextureFormat::RGB), GL_UNSIGNED_BYTE, faces[5]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, faceWidth, faceHeight, 0, InfinitToOpenGLTextureFormat(TextureFormat::RGB), GL_UNSIGNED_BYTE, faces[1]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, faceWidth, faceHeight, 0, InfinitToOpenGLTextureFormat(TextureFormat::RGB), GL_UNSIGNED_BYTE, faces[3]);

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			for (size_t i = 0; i < faces.size(); i++)
				delete[] faces[i];

			stbi_image_free(data);

			})
	}

	void OpenGLTextureCube::OnDeserialize(const json& json_object)
	{
		OpenGLTextureCube::OnDeserialize(json_object);

		int width;
		int height;
		int channels;
		byte* data = stbi_load(GetFilePath().c_str(), &width, &height, &channels, STBI_rgb_alpha);
		m_Width.SetValue(width);
		m_Height.SetValue(height);
		m_Channels.SetValue(channels);

		m_Format.SetValue((int) (m_Channels.GetValue() == 3 ? TextureFormat::RGB : TextureFormat::RGBA));

		CreateCubeMap(data);
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		IN_CORE_INFO("OpenGLTextureCube detroyed!");
		IN_RENDER_S({
				glDeleteTextures(1, &self->m_RendererID);
			})
	}

	void OpenGLTextureCube::Bind(uint slot) const
	{
		IN_RENDER_S1(slot, {
				glActiveTexture(GL_TEXTURE0 + slot);
				glBindTexture(GL_TEXTURE_CUBE_MAP, self->m_RendererID);
			})
	}

}

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
	OpenGLTexture2D::OpenGLTexture2D(const string& path, bool srgb)
		: Texture2D(path)
	{
		Reload(path);
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint width, uint height)
		: Texture2D(""), m_Format(format), m_Width(width), m_Height(height)
	{
		IN_RENDER_S({
			glGenTextures(1, &self->m_RendererID);
			glBindTexture(GL_TEXTURE_2D, self->m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, InfinitToOpenGLTextureFormat(self->m_Format), self->m_Width, self->m_Height, 0, InfinitToOpenGLTextureFormat(self->m_Format), GL_UNSIGNED_BYTE, NULL);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		})
	}

	bool OpenGLTexture2D::Reload(const string& filePath)
	{
		m_FilePath = filePath;
		int width, height, channels;
		IN_CORE_INFO("Loading texture {0}", m_FilePath);
		stbi_set_flip_vertically_on_load(false);
		m_ImageData = stbi_load(m_FilePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		IN_CORE_INFO("Ready reading data");

		m_Width = width;
		m_Height = height;
		m_Format = TextureFormat::RGBA;

		IN_RENDER_S({

				glGenTextures(1, &self->m_RendererID);
				glBindTexture(GL_TEXTURE_2D, self->m_RendererID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTexImage2D(GL_TEXTURE_2D, 0, InfinitToOpenGLTextureFormat(self->m_Format), self->m_Width, self->m_Height, 0, InfinitToOpenGLTextureFormat(self->m_Format), GL_UNSIGNED_BYTE, self->m_ImageData);
				glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, 0);

				stbi_image_free(self->m_ImageData);
			})
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

//REMOVE srgb??
	OpenGLTextureCube::OpenGLTextureCube(const string& path, bool srgb)
		: TextureCube(path), m_Width(0), m_Height(0)
	{
		Reload(path);
	}

	bool OpenGLTextureCube::Reload(const string& filePath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		m_ImageData = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb);

		m_Width = width;
		m_Height = height;
		m_Format = TextureFormat::RGB;
		m_FilePath = filePath;

		unsigned int faceWidth = m_Width / 4;
		unsigned int faceHeight = m_Height / 3;
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
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
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
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		IN_RENDER_S3(faces, faceWidth, faceHeight, {

			glGenTextures(1, &self->m_RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, self->m_RendererID);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			auto format = InfinitToOpenGLTextureFormat(self->m_Format);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[2]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[0]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[4]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[5]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[1]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[3]);

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			for (size_t i = 0; i < faces.size(); i++)
				delete[] faces[i];

			stbi_image_free(self->m_ImageData);
		})

		return true;
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

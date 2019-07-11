#pragma once

#include "Core/Core.h"
#include "inpch.h"
#include "glm/glm.hpp"
#include "graphics/VertexArray.h"
#include "graphics/Texture.h"
#include "Core/Material.h"

namespace Infinit {

	class Mesh
	{
	public:
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 Texcoord;
			glm::vec3 Tangent;
			glm::vec3 Binormal;
		};

		struct Index
		{
			uint V1, V2, V3;
		};

		Mesh(const string& filename);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices);
		~Mesh();

		inline const string& GetFilePath() const { return m_FilePath; }
		std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		std::shared_ptr<VertexArray> m_VertexArray;

		string m_FilePath;
	};

	class MeshInstance
	{
	public:
		MeshInstance(std::shared_ptr<Mesh> instance);

		inline std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }
		inline uint GetVertexCount() const { return m_VertexCount; }

		void DrawImGui();
	public:
		std::shared_ptr<Material> Material;
	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		uint m_VertexCount;
	};

}
#pragma once

namespace Infinit {

	class Mesh : public Resource
	{
		friend class MeshInstance;
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

		virtual bool Reload(const string& filepath);

		std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		std::shared_ptr<VertexArray> m_VertexArray;
	};

}
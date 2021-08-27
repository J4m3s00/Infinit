#pragma once

namespace Infinit {

	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 Texcoord;
		glm::vec3 Tangent;
		glm::vec3 Binormal;
	};

	struct MeshIndex
	{
		uint V1, V2, V3;
	};

	class Mesh : public Resource
	{
		friend class MeshInstance;
	public:
		Mesh();
		Mesh(const string& filename);
		Mesh(const std::vector<MeshVertex>& vertices, const std::vector<MeshIndex>& indices);
		~Mesh();

		virtual bool Reload(const string& filepath);
		virtual void OnDeserialize(const json& ref);

		std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }
	private:
		void CreateBuffers();
	private:
		ValueProperty<std::vector<MeshVertex>> m_Vertices;
		ValueProperty<std::vector<MeshIndex>> m_Indices;

		std::shared_ptr<VertexArray> m_VertexArray;
	};

}
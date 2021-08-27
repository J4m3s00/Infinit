#include "inpch.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace Infinit {

	namespace {
		const unsigned int ImportFlags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_PreTransformVertices |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_Debone |
			aiProcess_ValidateDataStructure;
	}

	struct LogStream : public Assimp::LogStream
	{
		static void Initialize()
		{
			if (Assimp::DefaultLogger::isNullLogger())
			{
				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
				Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
			}
		}

		void write(const char* message) override
		{
			IN_CORE_ERROR("Assimp error: {0}", message);
		}
	};
	
	Mesh::Mesh()
		: Resource("", Resource::Type::MESH),
		m_Vertices("Vertices", this, {}),
		m_Indices("Indicies", this, {})
	{
	}


	Mesh::Mesh(const string& filename)
		: Resource(filename, Resource::Type::MESH),
		m_Vertices("Vertices", this, {}),
		m_Indices("Indicies", this, {})
	{
		Reload(filename);
	}

	Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<MeshIndex>& indices)
		: Resource("", Resource::Type::MESH), m_Vertices("Vertices", this, vertices), m_Indices("Indices", this, indices)
	{
		m_VertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(m_Vertices->data(), m_Vertices->size() * sizeof(MeshVertex)));

		vertexBuffer->SetLayout({ {ShaderDataType::Float3, "position"},
									{ShaderDataType::Float3, "normal"},
									{ShaderDataType::Float2, "texCoord"},
									{ShaderDataType::Float3, "tangent"},
									{ShaderDataType::Float3, "binormal"} });
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create((const uint*)m_Indices->data(), m_Indices->size() * 3));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	bool Mesh::Reload(const string& filePath)
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
			LogStream::Initialize();

			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(filePath, ImportFlags);
			if (!scene || !scene->HasMeshes())
			{
				IN_CORE_ERROR("Failed to load mesh file: {0}", filePath);
				return false;
			}

			aiMesh* mesh = scene->mMeshes[0];

			IN_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
			IN_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

			m_Vertices->reserve(mesh->mNumVertices);

			// Extract vertices from model
			for (size_t i = 0; i < m_Vertices->capacity(); i++)
			{
				MeshVertex vertex;
				vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
				vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

				if (mesh->HasTangentsAndBitangents())
				{
					vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
					vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
				}

				if (mesh->HasTextureCoords(0))
					vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
				m_Vertices->push_back(vertex);
			}


			m_Indices->reserve(mesh->mNumFaces);
			for (size_t i = 0; i < m_Indices->capacity(); i++)
			{
				IN_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
				m_Indices->push_back({ mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] });
			}
		}

		
		return true;
	}

	Mesh::~Mesh()
	{
		IN_CORE_INFO("Deleted Mesh");
	}

	void Mesh::CreateBuffers()
	{
		m_VertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(m_Vertices->data(), m_Vertices->size() * sizeof(MeshVertex)));

		vertexBuffer->SetLayout({ {ShaderDataType::Float3, "position"},
									{ShaderDataType::Float3, "normal"},
									{ShaderDataType::Float2, "texCoord"},
									{ShaderDataType::Float3, "tangent"},
									{ShaderDataType::Float3, "binormal"} });
		m_VertexArray->AddVertexBuffer(vertexBuffer);




		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create((const uint*)&m_Indices.GetValue()[0], m_Indices->size() * 3));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void Mesh::OnDeserialize(const json& ref)
	{
		Resource::OnDeserialize(ref);
		CreateBuffers();
	}

}

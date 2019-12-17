#pragma once
namespace Infinit {

	class Transform
	{
	public:
		Transform();
		Transform(const glm::vec3& position, const glm::vec3& scale = glm::vec3(1.0f), const glm::vec3& rotation = glm::vec3())
			: m_Position(position), m_Scale(scale), m_Rotation(rotation), Position(m_Position), Scale(m_Scale), Rotation(m_Rotation)
		{
			CalculateTransformMatrix();
		}
		Transform(const Transform& ref);

		const glm::mat4& GetTransformMatrix() const { return m_TransformMatrix; }

		void SetPosition(const glm::vec3& position) { m_Position = position; }

		Transform& operator+(const Transform& transform);
		void operator+=(const Transform& transform);
		Transform& operator=(const Transform& transform);

		void DrawImGui();
	private:
		Transform* CalculateTransformMatrix();
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::vec3 m_Rotation;
		glm::mat4 m_TransformMatrix;
	public:
		const glm::vec3& Position;
		const glm::vec3& Scale;
		const glm::vec3& Rotation;
	};

}
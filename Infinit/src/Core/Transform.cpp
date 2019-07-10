#include "inpch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace Infinit {

	Transform::Transform()
		: Transform({}, { 1.0f, 1.0f, 1.0f }, {})
	{
		CalculateTransformMatrix();
	}

	Transform::Transform(const Transform& t)
		: m_Position(t.Position), m_Rotation(t.Rotation), m_Scale(t.Scale), Position(m_Position), Scale(m_Scale), Rotation(m_Rotation)
	{
		CalculateTransformMatrix();
	}

	Transform& Transform::operator+(const Transform& t)
	{
		m_Position += t.Position;
		m_Scale *= t.Scale;
		m_Rotation += t.Rotation;
		CalculateTransformMatrix();
		return *this;
	}

	Transform& Transform::operator=(const Transform& t)
	{
		m_Position = t.Position;
		m_Scale = t.Scale;
		m_Rotation = t.Rotation;
		CalculateTransformMatrix();
		return *this;
	}

	void Transform::operator+=(const Transform& t)
	{
		m_Position += t.Position;
		m_Rotation += t.Rotation;
		m_Scale *= t.Scale;
	}

	Transform* Transform::CalculateTransformMatrix()
	{
		m_TransformMatrix = glm::mat4(1.0f);
		m_TransformMatrix = glm::translate(m_TransformMatrix, m_Position);
		m_TransformMatrix = glm::scale(m_TransformMatrix, m_Scale);
		m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.x, { 1.0f, 0.0f, 0.0f });
		m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.y, { 0.0f, 1.0f, 0.0f });
		m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.z, { 0.0f, 0.0f, 1.0f });
		return this;
	}

	void Transform::DrawImGui()
	{
		ImGui::DragFloat3("Position", &m_Position[0]);
		ImGui::DragFloat3("Rotation", &m_Rotation[0]);
		ImGui::DragFloat3("Scale", &m_Scale[0]);
		CalculateTransformMatrix();
	}

}
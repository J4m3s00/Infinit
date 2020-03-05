#pragma once

namespace Infinit {
	namespace JsonHelper {
		json ConvertValue(int value);
		json ConvertValue(int* value);
		json ConvertValue(float value);
		json ConvertValue(float* value);
		json ConvertValue(double value);
		json ConvertValue(double* value);
		json ConvertValue(long value);
		json ConvertValue(long* value);
		json ConvertValue(const glm::vec2& value);
		json ConvertValue(glm::vec2* value);
		json ConvertValue(const glm::vec3& value);
		json ConvertValue(glm::vec3* value);
		json ConvertValue(const glm::vec4& value);
		json ConvertValue(glm::vec4* value);
		json ConvertValue(const glm::mat4& value);
		json ConvertValue(glm::mat4* value);
		json ConvertValue(const string& value);
		json ConvertValue(string* value);
	}
}
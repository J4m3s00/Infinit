#pragma once

namespace Infinit {
	namespace JsonHelper {
		bool HasObjectProperty(const char* paramName, const json& js_object);

		int GetParamInt(const char* paramName, const json& js_object);
		bool GetParamIntSafe(const char* paramName, const json& js_object, int& outValue);
		float GetParamFloat(const char* paramName, const json& js_object);
		bool GetParamFloatSafe(const char* paramName, const json& js_object, float& outValue);
		double GetParamDouble(const char* paramName, const json& js_object);
		bool GetParamDoubleSafe(const char* paramName, const json& js_object, double& outValue);
		long GetParamLong(const char* paramName, const json& js_object);
		bool GetParamLongSafe(const char* paramName, const json& js_object, long& outValue);
		glm::vec2 GetParamVec2(const char* paramName, const json& js_object);
		bool GetParamVec2Safe(const char* paramName, const json& js_object, glm::vec2& outValue);
		glm::vec3 GetParamVec3(const char* paramName, const json& js_object);
		bool GetParamVec3Safe(const char* paramName, const json& js_object, glm::vec3& outValue);
		glm::vec4 GetParamVec4(const char* paramName, const json& js_object);
		bool GetParamVec4Safe(const char* paramName, const json& js_object, glm::vec4& outValue);
		glm::mat4 GetParamMat4(const char* paramName, const json& js_object);
		bool GetParamMat4Safe(const char* paramName, const json& js_object, glm::mat4& outValue);


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
#pragma once

namespace Infinit {

	struct MeshVertex; 
	struct MeshIndex;

	namespace JsonHelper {
		bool HasObjectProperty(const char* paramName, const json& js_object);

		int GetParamInt(const char* paramName, const json& js_object);
		bool GetParamIntSafe(const char* paramName, const json& js_object, int& outValue);
		uint GetParamUInt(const char* paramName, const json& js_object);
		bool GetParamUIntSafe(const char* paramName, const json& js_object, uint& outValue);
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
		UUID GetParamUUID(const char* paramName, const json& js_object);
		bool GetParamUUIDSafe(const char* paramName, const json& js_object, UUID& outValue);
		string GetParamString(const char* paramName, const json& js_object);
		bool GetParamStringSafe(const char* paramName, const json& js_object, string& outValue);
		MeshVertex GetParamMeshVertex(const char* paramName, const json& js_object);
		bool GetParamMeshVertexSafe(const char* paramName, const json& js_object, MeshVertex& outValue);
		MeshIndex GetParamMeshIndex(const char* paramName, const json& js_object);
		bool GetParamMeshIndexSafe(const char* paramName, const json& js_object, MeshIndex& outValue);

		json ConvertValue(int value);
		json ConvertValue(int* value);
		json ConvertValue(uint value);
		json ConvertValue(uint* value);
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
		json ConvertValue(const UUID& value);
		json ConvertValue(UUID* value);
		json ConvertValue(const MeshVertex& value);
		json ConvertValue(MeshVertex* value);
		json ConvertValue(const MeshIndex& value);
		json ConvertValue(MeshIndex* value);


		void ConvertObject(const json& ref, int& value);
		void ConvertObject(const json& ref, int* value);
		void ConvertObject(const json& ref, uint& value);
		void ConvertObject(const json& ref, uint* value);
		void ConvertObject(const json& ref, float& value);
		void ConvertObject(const json& ref, float* value);
		void ConvertObject(const json& ref, double& value);
		void ConvertObject(const json& ref, double* value);
		void ConvertObject(const json& ref, long& value);
		void ConvertObject(const json& ref, long* value);
		void ConvertObject(const json& ref, glm::vec2& value);
		void ConvertObject(const json& ref, glm::vec2* value);
		void ConvertObject(const json& ref, glm::vec3& value);
		void ConvertObject(const json& ref, glm::vec3* value);
		void ConvertObject(const json& ref, glm::vec4& value);
		void ConvertObject(const json& ref, glm::vec4* value);
		void ConvertObject(const json& ref, glm::mat4& value);
		void ConvertObject(const json& ref, glm::mat4* value);
		void ConvertObject(const json& ref, string& value);
		void ConvertObject(const json& ref, string* value);
		void ConvertObject(const json& ref, UUID& value);
		void ConvertObject(const json& ref, UUID* value);
		void ConvertObject(const json& ref, MeshVertex& value);
		void ConvertObject(const json& ref, MeshVertex* value);
		void ConvertObject(const json& ref, MeshIndex& value);
		void ConvertObject(const json& ref, MeshIndex* value);


		template <typename T>
		json ConvertValue(const std::vector<T>& value)
		{
			json result = json::array();
			for (const T& v : value)
			{
				result.push_back(ConvertValue(v));
			}
			return result;
		}

		template <typename T>
		json ConvertValue(const std::vector<T>* value)
		{
			json result = json::array();
			for (const T& v : value)
			{
				result.push_back(ConvertValue(v));
			}
			return result;
		}

		template <typename T>
		void ConvertObject(const json& ref, std::vector<T>& outValues)
		{
			if (!ref.is_array()) { IN_CORE_WARN("Could not convert json to vector. Json is not a array: \n{}", ref.dump()); return; }
			outValues.clear();
			outValues.resize(ref.size());
			int index = 0;
			for (const json& js : ref) 
			{
				ConvertObject(js, &outValues[index++]);
			}
		}

		template <typename T>
		void ConvertObject(const json& ref, std::vector<T>* outValues)
		{
			if (!ref.is_array()) { IN_CORE_WARN("Could not convert json to vector. Json is not a array: \n{}", ref.dump()); return; }
			outValues.clear();
			outValues.resize(ref.size());
			int index = 0;
			for (const json& js : ref)
			{
				ConvertObject(js, &outValues[index++]);
			}
		}
	}
}
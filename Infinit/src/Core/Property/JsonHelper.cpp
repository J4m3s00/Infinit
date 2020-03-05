#include "inpch.h"

namespace Infinit { namespace JsonHelper {

	bool HasObjectProperty(const char* paramName, const json& js_object)
	{
		return js_object.find(paramName) != js_object.end();
	}

	int GetParamInt(const char* paramName, const json& js_object)
	{
		int result = 0;
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_number_integer()) { IN_CORE_WARN("Javascript value is not an Integer!"); IN_CORE_WARN("Object: ", jsParam.dump()); return 0; }
		result = jsParam.get<int>();
		return result;
	}

	bool GetParamIntSafe(const char* paramName, const json& js_object, int& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = 0;
			return false;
		}
		outValue = GetParamInt(paramName, js_object);
		return true;
	}

	float GetParamFloat(const char* paramName, const json& js_object)
	{
		float result = 0;
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_number_float()) { IN_CORE_WARN("Javascript value is not a Float!"); IN_CORE_WARN("Object: ", jsParam.dump()); return 0.0; }
		result = jsParam.get<float>();
		return result;
	}

	bool GetParamFloatSafe(const char* paramName, const json& js_object, float& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = 0.0;
			return false;
		}
		outValue = GetParamFloat(paramName, js_object);
		return true;
	}

	double GetParamDouble(const char* paramName, const json& js_object)
	{
		double result = 0;
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_number()) { IN_CORE_WARN("Javascript value is not a Number!"); IN_CORE_WARN("Object: ", jsParam.dump()); return 0.0; }
		result = jsParam.get<double>();
		return result;
	}

	bool GetParamDoubleSafe(const char* paramName, const json& js_object, double& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = 0.0;
			return false;
		}
		outValue = GetParamDouble(paramName, js_object);
		return true;
	}


	long GetParamLong(const char* paramName, const json& js_object)
	{
		long result = 0;
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_number()) { IN_CORE_WARN("Javascript value is not a Number!"); IN_CORE_WARN("Object: ", jsParam.dump()); return 0.0; }
		result = jsParam.get<long>();
		return result;
	}

	bool GetParamLongSafe(const char* paramName, const json& js_object, long& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = 0.0;
			return false;
		}
		outValue = GetParamLong(paramName, js_object);
		return true;
	}

	glm::vec2 GetParamVec2(const char* paramName, const json& js_object)
	{
		glm::vec2 result;
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_object()) { IN_CORE_WARN("JSON vec2 is not an JsonObject!"); IN_CORE_WARN("Object: ", jsParam.dump()); return result; }
		GetParamFloatSafe("X", jsParam, result.x);
		GetParamFloatSafe("Y", jsParam, result.y);
		return result;
	}

	bool GetParamVec2Safe(const char* paramName, const json& js_object, glm::vec2& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = glm::vec2();
			return false;
		}
		outValue = GetParamVec2(paramName, js_object);
		return true;
	}

	glm::vec3 GetParamVec3(const char* paramName, const json& js_object)
	{
		glm::vec3 result;
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_object()) { IN_CORE_WARN("JSON vec3 is not an JsonObject!"); IN_CORE_WARN("Object: ", jsParam.dump()); return result; }
		GetParamFloatSafe("X", jsParam, result.x);
		GetParamFloatSafe("Y", jsParam, result.y);
		GetParamFloatSafe("Z", jsParam, result.z);
		return result;
	}

	bool GetParamVec3Safe(const char* paramName, const json& js_object, glm::vec3& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = glm::vec3();
			return false;
		}
		outValue = GetParamVec3(paramName, js_object);
		return true;
	}

	glm::vec4 GetParamVec4(const char* paramName, const json& js_object)
	{
		glm::vec4 result;
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_object()) { IN_CORE_WARN("JSON vec4 is not an JsonObject!"); IN_CORE_WARN("Object: ", jsParam.dump()); return result; }
		GetParamFloatSafe("X", jsParam, result.x);
		GetParamFloatSafe("Y", jsParam, result.y);
		GetParamFloatSafe("Z", jsParam, result.z);
		return result;
	}

	bool GetParamVec4Safe(const char* paramName, const json& js_object, glm::vec4& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = glm::vec4();
			return false;
		}
		outValue = GetParamVec4(paramName, js_object);
		return true;
	}

	glm::mat4 GetParamMat4(const char* paramName, const json& js_object)
	{
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_array()) { IN_CORE_WARN("JSON mat4 is not an JsonArray!"); IN_CORE_WARN("Object: ", jsParam.dump()); return glm::mat4(1.0); }
		
		return glm::mat4(jsParam[0].get<float>(),  jsParam[1].get<float>(),  jsParam[2].get<float>(),  jsParam[3].get<float>(),
						 jsParam[4].get<float>(),  jsParam[5].get<float>(),  jsParam[6].get<float>(),  jsParam[7].get<float>(), 
						 jsParam[8].get<float>(),  jsParam[9].get<float>(),  jsParam[10].get<float>(), jsParam[11].get<float>(), 
						 jsParam[12].get<float>(), jsParam[13].get<float>(), jsParam[14].get<float>(), jsParam[15].get<float>());
	}

	bool GetParamMat4Safe(const char* paramName, const json& js_object, glm::mat4& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = glm::mat4();
			return false;
		}
		outValue = GetParamMat4(paramName, js_object);
		return true;
	}

	json ConvertValue(int value)
	{
		return value;
	}

	json ConvertValue(int* value)
	{
		return *value;
	}

	json ConvertValue(float value)
	{
		return value;
	}

	json ConvertValue(float* value)
	{
		return *value;
	}

	json ConvertValue(double value)
	{
		return value;
	}

	json ConvertValue(double* value)
	{
		return *value;
	}
	
	json ConvertValue(long value)
	{
		return value;
	}

	json ConvertValue(long* value)
	{
		return *value;
	}

	json ConvertValue(const glm::vec2& value)
	{
		json result = json::object();

		result["X"] = value.x;
		result["Y"] = value.y;

		return result;
	}

	json ConvertValue(glm::vec2* value)
	{
		json result = json::object();

		result["X"] = value->x;
		result["Y"] = value->y;

		return result;
	}

	json ConvertValue(const glm::vec3& value)
	{
		json result = json::object();

		result["X"] = value.x;
		result["Y"] = value.y;
		result["Z"] = value.z;

		return result;
	}

	json ConvertValue(glm::vec3* value)
	{
		json result = json::object();

		result["X"] = value->x;
		result["Y"] = value->y;
		result["Z"] = value->z;

		return result;
	}

	json ConvertValue(const glm::vec4& value)
	{
		json result = json::object();

		result["X"] = value.x;
		result["Y"] = value.y;
		result["Z"] = value.z;
		result["W"] = value.w;

		return result;
	}

	json ConvertValue(glm::vec4* value)
	{
		json result = json::object();

		result["X"] = value->x;
		result["Y"] = value->y;
		result["Z"] = value->z;
		result["W"] = value->w;

		return result;
	}


	json ConvertValue(const glm::mat4& value)
	{
		json result = json::array();

		for (int i = 0; i < 4 * 4; i++) {
			result.push_back(ConvertValue(value[i]));
		}

		return result;
	}

	json ConvertValue(glm::mat4* value)
	{
		json result = json::array();

		for (int i = 0; i < 4 * 4; i++) {
			result.push_back(ConvertValue(value[i]));
		}

		return result;
	}

	json ConvertValue(const string& value)
	{
		return value;
	}

	json ConvertValue(string* value)
	{
		return *value;
	}

} }
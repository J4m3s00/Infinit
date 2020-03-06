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
		if (!jsParam.is_number_integer()) { IN_CORE_WARN("Javascript value is not an Integer!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return 0; }
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
		if (!jsParam.is_number_float()) { IN_CORE_WARN("Javascript value is not a Float!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return 0.0; }
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
		if (!jsParam.is_number()) { IN_CORE_WARN("Javascript value is not a Number!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return 0.0; }
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
		if (!jsParam.is_number()) { IN_CORE_WARN("Javascript value is not a Number!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return 0.0; }
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
		glm::vec2 result(0.0f);
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_object()) { IN_CORE_WARN("JSON vec2 is not an JsonObject!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return result; }
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
		glm::vec3 result(0.0f);
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_object()) { IN_CORE_WARN("JSON vec3 is not an JsonObject!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return result; }
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
		glm::vec4 result(0.0f);
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_object()) { IN_CORE_WARN("JSON vec4 is not an JsonObject!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return result; }
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
		if (!jsParam.is_array()) { IN_CORE_WARN("JSON mat4 is not an JsonArray!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return glm::mat4(1.0); }
		
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

	UUID GetParamUUID(const char* paramName, const json& js_object)
	{
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_string()) { IN_CORE_WARN("JSON uuid is not an JsonString!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return UUID(); }

		string uuidString = GetParamString(paramName, js_object);
		UUID result;
		result.FromString(uuidString);
		return result;
	}

	bool GetParamUUIDSafe(const char* paramName, const json& js_object, UUID& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = UUID();
			return false;
		}
		outValue = GetParamUUID(paramName, js_object);
		return true;
	}

	string GetParamString(const char* paramName, const json& js_object)
	{
		const json& jsParam = js_object[paramName];
		if (!jsParam.is_string()) { IN_CORE_WARN("JSON uuid is not an JsonString!"); IN_CORE_WARN("Object: {}", jsParam.dump()); return ""; }

		string result = GetParamString(paramName, js_object);
		return result;
	}

	bool GetParamStringSafe(const char* paramName, const json& js_object, string& outValue)
	{
		if (!HasObjectProperty(paramName, js_object))
		{
			outValue = "";
			return false;
		}
		outValue = GetParamString(paramName, js_object);
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

	json ConvertValue(const UUID& value)
	{
		return value.ToString();
	}

	json ConvertValue(UUID* value)
	{
		return value->ToString();
	}

	void ConvertObject(const json& ref, int& value)
	{
		IN_CORE_ASSERT(ref.is_number(), "Json value  {}   is not a number!", ref.dump());
		if (ref.is_number()) { value = ref.get<int>(); }
	}

	void ConvertObject(const json& ref, int* value)
	{
		IN_CORE_ASSERT(ref.is_number(), "Json value  {}   is not a number!", ref.dump());
		if (ref.is_number()) { *value = ref.get<int>(); }
	}

	void ConvertObject(const json& ref, float& value)
	{
		IN_CORE_ASSERT(ref.is_number(), "Json value  {}   is not a number!", ref.dump());
		if (ref.is_number()) { value = ref.get<float>(); }
	}

	void ConvertObject(const json& ref, float* value)
	{
		IN_CORE_ASSERT(ref.is_number(), "Json value  {}   is not a number!", ref.dump());
		if (ref.is_number()) { *value = ref.get<float>(); }
	}

	void ConvertObject(const json& ref, double& value)
	{
		IN_CORE_ASSERT(ref.is_number(), "Json value  {}   is not a number!", ref.dump());
		if (ref.is_number()) { value = ref.get<double>(); }
	}

	void ConvertObject(const json& ref, double* value)
	{
		IN_CORE_ASSERT(ref.is_number(), "Json value  {}   is not a number!", ref.dump());
		if (ref.is_number()) { *value = ref.get<double>(); }
	}

	void ConvertObject(const json& ref, long& value)
	{
		IN_CORE_ASSERT(ref.is_number(), "Json value  {}   is not a number!", ref.dump());
		if (ref.is_number()) { value = ref.get<long>(); }
	}

	void ConvertObject(const json& ref, long* value)
	{
		IN_CORE_ASSERT(ref.is_number(), "Json value  {}   is not a number!", ref.dump());
		if (ref.is_number()) { *value = ref.get<long>(); }
	}

	void ConvertObject(const json& ref, glm::vec2& value)
	{
		IN_CORE_ASSERT(ref.is_object(), "Json value  {}   is not a Object!", ref.dump());
		if (ref.is_object())
		{ 
			GetParamFloatSafe("X", ref, value.x);
			GetParamFloatSafe("Y", ref, value.y);
		}
	}

	void ConvertObject(const json& ref, glm::vec2* value)
	{
		IN_CORE_ASSERT(ref.is_object(), "Json value  {}   is not a Object!", ref.dump());
		if (ref.is_object())
		{
			GetParamFloatSafe("X", ref, value->x);
			GetParamFloatSafe("Y", ref, value->y);
		}
	}

	void ConvertObject(const json& ref, glm::vec3& value)
	{
		IN_CORE_ASSERT(ref.is_object(), "Json value  {}   is not a Object!", ref.dump());
		if (ref.is_object())
		{
			GetParamFloatSafe("X", ref, value.x);
			GetParamFloatSafe("Y", ref, value.y);
			GetParamFloatSafe("Z", ref, value.z);
		}
	}

	void ConvertObject(const json& ref, glm::vec3* value)
	{
		IN_CORE_ASSERT(ref.is_object(), "Json value  {}   is not a Object!", ref.dump());
		if (ref.is_object())
		{
			GetParamFloatSafe("X", ref, value->x);
			GetParamFloatSafe("Y", ref, value->y);
			GetParamFloatSafe("Z", ref, value->z);
		}
	}

	void ConvertObject(const json& ref, glm::vec4& value)
	{
		IN_CORE_ASSERT(ref.is_object(), "Json value  {}   is not a Object!", ref.dump());
		if (ref.is_object())
		{
			GetParamFloatSafe("X", ref, value.x);
			GetParamFloatSafe("Y", ref, value.y);
			GetParamFloatSafe("Z", ref, value.z);
			GetParamFloatSafe("W", ref, value.w);
		}
	}

	void ConvertObject(const json& ref, glm::vec4* value)
	{
		IN_CORE_ASSERT(ref.is_object(), "Json value  {}   is not a Object!", ref.dump());
		if (ref.is_object())
		{
			GetParamFloatSafe("X", ref, value->x);
			GetParamFloatSafe("Y", ref, value->y);
			GetParamFloatSafe("Z", ref, value->z);
			GetParamFloatSafe("W", ref, value->w);
		}
	}

	void ConvertObject(const json& ref, glm::mat4& value)
	{
		IN_CORE_ASSERT(ref.is_array(), "Json value  {}   is not a Matrix!", ref.dump());
		if (ref.is_array())
		{
			value = glm::mat4(ref[0].get<float>(), ref[1].get<float>(), ref[2].get<float>(), ref[3].get<float>(),
				ref[4].get<float>(), ref[5].get<float>(), ref[6].get<float>(), ref[7].get<float>(),
				ref[8].get<float>(), ref[9].get<float>(), ref[10].get<float>(), ref[11].get<float>(),
				ref[12].get<float>(), ref[13].get<float>(), ref[14].get<float>(), ref[15].get<float>());
		}
	}

	void ConvertObject(const json& ref, glm::mat4* value)
	{
		IN_CORE_ASSERT(ref.is_array(), "Json value  {}   is not a Matrix!", ref.dump());
		if (ref.is_array())
		{
			*value = glm::mat4(ref[0].get<float>(), ref[1].get<float>(), ref[2].get<float>(), ref[3].get<float>(),
				ref[4].get<float>(), ref[5].get<float>(), ref[6].get<float>(), ref[7].get<float>(),
				ref[8].get<float>(), ref[9].get<float>(), ref[10].get<float>(), ref[11].get<float>(),
				ref[12].get<float>(), ref[13].get<float>(), ref[14].get<float>(), ref[15].get<float>());
		}
	}

	void ConvertObject(const json& ref, string& value)
	{
		IN_CORE_ASSERT(ref.is_string(), "Json value  {}   is not a String!", ref.dump());
		if (ref.is_string())
		{
			value = ref.get<string>();
		}
	}

	void ConvertObject(const json& ref, string* value)
	{
		IN_CORE_ASSERT(ref.is_string(), "Json value  {}   is not a String!", ref.dump());
		if (ref.is_string())
		{
			*value = ref.get<string>();
		}
	}

	void ConvertObject(const json& ref, UUID& value)
	{
		IN_CORE_ASSERT(ref.is_string(), "Json value  {}   is not a String!", ref.dump());
		if (ref.is_string())
		{
			string uuidString = ref.get<string>();
			(value).FromString(uuidString);
		}
	}

	void ConvertObject(const json& ref, UUID* value)
	{
		IN_CORE_ASSERT(ref.is_string(), "Json value  {}   is not a String!", ref.dump());
		if (ref.is_string())
		{
			string uuidString = ref.get<string>();
			(*value).FromString(uuidString);
		}
	}



} }
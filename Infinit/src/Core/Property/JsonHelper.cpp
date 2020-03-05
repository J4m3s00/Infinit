#include "inpch.h"

namespace Infinit { namespace JsonHelper {

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
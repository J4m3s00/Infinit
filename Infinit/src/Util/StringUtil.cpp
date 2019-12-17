#include "inpch.h"


namespace Infinit {

	std::vector<string> SplitString(const string& str, const string& delimiters)
	{

		size_t start = 0;
		size_t end = str.find_first_of(delimiters);

		std::vector<string> result;

		while (end <= string::npos)
		{
			string token = str.substr(start, end - start);
			if (!token.empty())
				result.push_back(token);

			if (end == string::npos)
				break;

			start = end + 1;
			end = str.find_first_of(delimiters, start);
		}

		return result;
	}

	std::vector<string> SplitString(const string& str, const char delimiter)
	{
		return SplitString(str, string(1, delimiter));
	}

	std::vector<string> Tokenize(const string& str)
	{
		std::vector<std::string> tokens;
		std::istringstream iss(str);
		for (string s; iss >> s; )
			tokens.push_back(s);

		return tokens;
		//return SplitString(str, " \t\n");
	}

	std::vector<string> GetLines(const string& str)
	{
		return SplitString(str, "\n");
	}

	const char* FindToken(const char* str, const string& token)
	{
		const char* t = str;
		while (t = strstr(t, token.c_str()))
		{
			bool left = str == t || isspace(t[-1]);
			bool right = !t[token.size()] || isspace(t[token.size()]);
			if (left && right)
				return t;

			t += token.size();
		}
		return nullptr;
	}

	const char* FindToken(const string& str, const string& token)
	{
		return FindToken(str.c_str(), token);
	}


	string GetStatement(const char* str, const char** outPosition)
	{
		const char* end = strstr(str, ";");
		if (!end)
			return string(str);

		if (outPosition)
			*outPosition = end;
		uint length = end - str + 1;
		return string(str, length);
	}

	string GetBlock(const char* str, const char** outPosition)
	{
		const char* end = strstr(str, "}");
		if (!end)
			return string(str);

		if (outPosition)
			*outPosition = end;
		uint length = end - str + 1;
		return string(str, length);
	}

	string GetBlock(const string& string, uint offset)
	{
		const char* str = string.c_str() + offset;
		return GetBlock(str);
	}

}
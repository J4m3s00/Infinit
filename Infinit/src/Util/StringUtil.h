#pragma once

namespace Infinit {

	std::vector<string> SplitString(const string& str, const string& delimiters);
	std::vector<string> SplitString(const string& str, const char delimiter);
	std::vector<string> Tokenize(const string& str);
	std::vector<string> GetLines(const string& str);

	const char* FindToken(const char* str, const string& token);
	const char* FindToken(const string& str, const string& token);
	string GetStatement(const char* str, const char** outPosition = nullptr);

	string GetBlock(const char* str, const char** outPosition = nullptr);
	string GetBlock(const string& string, uint offset = 0);

}
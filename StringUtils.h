#pragma once
#include <string>
class StringUtils
{
public:
	static std::string format(const char* format, ...);
	static std::wstring convert(const char* utf8);
	static std::string convert(const wchar_t* utf16, LPCCH lpDefaultChar = NULL, LPBOOL pfUsedDefaultChar = NULL);
};


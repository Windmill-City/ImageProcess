#include "pch.h"
#include "StringUtils.h"
#include "FormattedException.h"

std::string StringUtils::format(const char* format, ...)
{
    va_list list;
    va_start(list, format);
    //��ȡ��ʽ�����ַ����ĳ���
    int size = vsnprintf(NULL, 0, format, list);
    va_end(list);
    if (size < 0) {
        throw FormattedException("Error Formatting String: %s", format);
    }

    //��λva_list������ʽ���ַ���д�뵽buf
    va_start(list, format);
    auto result = std::string(++size, 0);
    vsnprintf(&result[0], size, format, list);
    va_end(list);
    return result;
}

std::wstring StringUtils::convert(const char* utf8)
{
    size_t size = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    auto result = std::wstring(++size, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8, (int)size, &result[0], (int)size);
    return result;
}

std::string StringUtils::convert(const wchar_t* utf16, LPCCH lpDefaultChar, LPBOOL pfUsedDefaultChar)
{
    size_t size = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, lpDefaultChar, pfUsedDefaultChar);
    auto result = std::string(++size, 0);
    WideCharToMultiByte(CP_UTF8, 0, utf16, (int)size, &result[0], (int)size, lpDefaultChar, pfUsedDefaultChar);
    return result;
}

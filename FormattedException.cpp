#include "pch.h"
#include "StringUtils.h"
#include "FormattedException.h"

FormattedException::FormattedException(char* format, ...)
{
	va_list list;
	va_start(list, format);
	What = std::move(StringUtils::format(format, list));
	va_end(list);
}

char const* FormattedException::what() const
{
	return What.c_str();
}

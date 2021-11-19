#pragma once
#include <string>
#include <exception>
class FormattedException : public std::exception
{
	std::string What;
public:
	FormattedException(char* format, ...);
	virtual char const* what() const override;
};


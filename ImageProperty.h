#pragma once
#include <string>
class ImageProperty
{
};

template <typename T>
class ImagePropertyOf : public ImageProperty
{
public:
	T Value;
	ImagePropertyOf(T value) : Value(value) {};
};


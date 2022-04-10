#pragma once
#include <string>
#include "Graphics.h"

class Resource
{
public:
	Resource(const wchar_t* _fullPath, Graphics* graphics);
	~Resource() = default;

protected:

	std::wstring m_fullPath;
};


#pragma once
#include "pch.h"
#include <stdexcept>
#include <string>

namespace Error
{
	inline void ThrowIfFailed(HRESULT hr, std::string message)
	{
		if (FAILED(hr))
		{
			throw std::runtime_error(std::string("Error: ") + message);
		}
	}
}
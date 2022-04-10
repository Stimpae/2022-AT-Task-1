#pragma once
#include "Resource.h"
#include <memory>
#include <unordered_map>
#include "Graphics.h"

class ResourceManager
{
public:
	ResourceManager() = default;
	~ResourceManager() = default;

	Resource* CreateResourceFromFile(const wchar_t* _filePath, Graphics* graphics);

protected:

	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* _filePath, Graphics* graphics) = 0;

private:

	std::unordered_map<std::wstring, Resource*> m_mapResources;
};


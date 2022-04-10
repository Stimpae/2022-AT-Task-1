#include "TextureManager.h"

Texture* TextureManager::CreateTextureFromFIle(const wchar_t* _filePath, Graphics* graphics)
{
	return static_cast<Texture*>(CreateResourceFromFile(_filePath, graphics));
}

Resource* TextureManager::CreateResourceFromFileConcrete(const wchar_t* _filePath, Graphics* graphics)
{
	Texture* texture = nullptr;
	if (texture = new Texture(_filePath, graphics))
	{
		return texture;
	}

	return nullptr;
}

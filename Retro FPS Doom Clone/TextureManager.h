#pragma once
#include "ResourceManager.h"
#include "Texture.h"


class TextureManager : public ResourceManager
{
public:
	TextureManager() = default;
	~TextureManager() = default;
	Texture* CreateTextureFromFIle(const wchar_t* _filePath, Graphics* graphics);

protected:

	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* _filePath, Graphics* graphics) override;

private:

};


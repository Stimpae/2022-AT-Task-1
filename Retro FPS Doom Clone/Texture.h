#pragma once
#include "pch.h"
#include "Resource.h"

class Texture : public Resource
{
public:
	Texture(const wchar_t* _fullPath, Graphics* graphics);
	~Texture() = default;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView() { return m_shaderResView; };

private:

	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResView;
};


#include "Texture.h"
#include "pch.h"
#include <DirectXTex.h>
#include "ErrorHelper.h"

Texture::Texture(const wchar_t* _fullPath, Graphics* graphics) : Resource(_fullPath, graphics)
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		throw std::exception("Texture co initialise failed");
	}

	DirectX::ScratchImage imageData;
	HRESULT res = DirectX::LoadFromWICFile(_fullPath, DirectX::WIC_FLAGS_NONE, nullptr, imageData);
	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(graphics->GetDevice(), imageData.GetImages(), imageData.GetImageCount(), imageData.GetMetadata(), &m_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = imageData.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = imageData.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		graphics->GetDevice()->CreateShaderResourceView(m_texture.Get(), &desc, &m_shaderResView);
	}
	else
	{
		throw std::exception("Texture not created");
	}
}

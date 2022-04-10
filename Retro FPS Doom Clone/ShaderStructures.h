#pragma once
#include "pch.h"

struct ModelViewProjectionConstantBuffer 
{
	DirectX::XMFLOAT4X4 model; // model scale , position, rotation
	DirectX::XMFLOAT4X4 view; //camera view
	DirectX::XMFLOAT4X4 projection; //cam projection
};

struct VertexPositionTexture
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 tex;
};
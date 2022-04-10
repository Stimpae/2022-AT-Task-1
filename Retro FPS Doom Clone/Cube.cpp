#include "Cube.h"

void Cube::Render(Renderer* _renderer, Camera* _cam)
{
	// Set the shape, postion and scale 
	DirectX::XMMATRIX shape = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX shapePos = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	DirectX::XMMATRIX shapeScale = DirectX::XMMatrixScaling(m_scale.x,m_scale.y, m_scale.z);

	// calculate the shape matrix	
	shape = shapeScale * shapePos;
	
	// Set the constant buffer object shape matrix here 
	DirectX::XMStoreFloat4x4(&m_constantObjectData.model, DirectX::XMMatrixTranspose(shape));

	// Set the constant buffer object camera projection matrix here
	DirectX::XMStoreFloat4x4(&m_constantObjectData.projection, DirectX::XMMatrixTranspose(_cam->GetProjectionMatrix()));

	// Set the constant buffer object camera view projection matrix here
	DirectX::XMStoreFloat4x4(&m_constantObjectData.view, DirectX::XMMatrixTranspose(_cam->GetViewMatrix()));
	
	// Update the constant buffer with the new object to be rendered to the screen
	// We need to be able to manually say the index amount we want to request from the vertex buffer
	_renderer->UpdateConstantBuffer(m_constantObjectData, m_indexcount);
}

void Cube::Update()
{
}

void Cube::SetTexture(Renderer* _renderer, Texture* _texture)
{
	_renderer->UpdatePixelShaderResources(_texture->GetResourceView());
}


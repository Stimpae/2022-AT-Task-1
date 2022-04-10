#pragma once
#include "Camera.h"
#include "pch.h"
#include "Renderer.h"
#include "ShaderStructures.h"

class Billboard
{
public:

	Billboard() = default;
	~Billboard() = default;

	void Render(Renderer * _renderer, Camera * _cam);
	void Update();

	void SetPosition(DirectX::XMFLOAT3 _position) { m_position = _position; };
	void SetScale(DirectX::XMFLOAT3 _scale) { m_scale = _scale; };
	void SetRotation(DirectX::XMFLOAT3 _rotation) { m_rotation = _rotation; };
	void SetTexture(Renderer * _renderer, Texture * _texture);

	DirectX::XMFLOAT3 GetPosition() { return m_position; };
	DirectX::XMFLOAT3 GetScale() { return m_scale; };
	DirectX::XMFLOAT3 GetRotation() { return m_rotation; };

	// a bit of a hacky way for collision
	bool m_active = false;

private:

	DirectX::XMFLOAT3 m_position = {};
	DirectX::XMFLOAT3 m_scale = {};
	DirectX::XMFLOAT3 m_rotation = {};

	unsigned int m_indexcount = 8;

	ModelViewProjectionConstantBuffer m_constantObjectData{};
};


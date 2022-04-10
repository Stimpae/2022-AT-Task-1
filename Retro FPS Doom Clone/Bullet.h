#pragma once
#include "pch.h"

class Cube;
class Renderer;
class Camera;
class Texture;

class Bullet
{
public:
	Bullet() = default;
	~Bullet();

	void Shoot(Camera* camera);
	void Render(Renderer* renderer, Camera* camera, double delta);
	void SetTexture(Renderer* renderer, Texture* texture);

	DirectX::XMFLOAT3 GetPosition() { return m_position; };
	DirectX::XMFLOAT3 GetScale() { return m_scale; };
	DirectX::XMFLOAT3 GetRotation() { return m_rotation; };

private:

	DirectX::XMFLOAT3 m_position = {};
	DirectX::XMFLOAT3 m_scale = {};
	DirectX::XMFLOAT3 m_rotation = {};

	Cube* m_bulletCube;
	float m_timeToDestroy = 5.0f;
	float m_timeAlive = 0.0f;
};


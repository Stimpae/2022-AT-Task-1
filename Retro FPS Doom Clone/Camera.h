#pragma once

#include "pch.h"
#include "Collider.h"
#include <memory>
#include "Texture.h"
#include <vector>

class Bullet;
class Renderer;

class Camera
{
public:
	Camera();
	~Camera();

	void InitializeOrthoMatrix(int _screenWidth, int _screenHeight, float _screenNear, float _screenDepth);
	void InitializeProjectionMatrix(float _fov, float _screenAspect, float _screenNear, float _screenDepth);

	void Update(DirectX::XMFLOAT3 _translation);
	void Render(Renderer* renderer, double delta);
	void Shoot(Texture* bulletTex);

	DirectX::XMFLOAT3  GetPosition()			{ return m_position; };
	DirectX::XMFLOAT3  GetRotation()			{ return m_rotation; };
	DirectX::XMMATRIX  GetViewMatrix()			{ return m_cameraViewMatrix; };
	DirectX::XMMATRIX  GetProjectionMatrix()	{ return m_cameraProjectionMatrix; };
	DirectX::XMVECTOR GetCameraTarget()			{ return m_cameraTarget; };


	void RotationCamera(DirectX::XMFLOAT3 _rotation);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	Collider* GetCollider() { return m_collider; };
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_lastPosition;


private:

	DirectX::XMVECTOR m_cameraPosition;
	DirectX::XMMATRIX m_cameraViewMatrix;
	DirectX::XMMATRIX m_cameraProjectionMatrix;

	DirectX::XMVECTOR m_cameraTarget;
	DirectX::XMVECTOR m_cameraUp;

	DirectX::XMFLOAT3 m_position{ 0.0f,0.7f, 1.5f};
	DirectX::XMFLOAT3 m_rotation{ 0.0f,0.0f,0.0f };
	
	DirectX::XMVECTOR m_defaultForward = { 0.0f, 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR m_defaultRight = { 1.0f, 0.0f, 0.0f, 0.0f };

	std::vector<Bullet*> m_bulletsFired;
	Texture* m_bulletTexture;

	// camera rotation
	float m_camYaw = 0.0f;

	Collider* m_collider;
};


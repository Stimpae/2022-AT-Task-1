#include "Camera.h"
#include <iostream>
#include "Bullet.h"
#include "Renderer.h"

Camera::Camera()
{
	m_collider = new Collider();
}

Camera::~Camera()
{
	delete m_collider;
}

// 2D Projection
void Camera::InitializeOrthoMatrix(int _screenWidth, int _screenHeight, float _screenNear, float _screenDepth)
{
}

// 3D Projection
void Camera::InitializeProjectionMatrix(float _fov, float _screenAspect, float _screenNear, float _screenDepth)
{
	// Sets the intiial values for each camera variable
	m_cameraPosition = DirectX::XMLoadFloat3(&m_position);
	m_cameraTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_cameraUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_cameraViewMatrix = DirectX::XMMatrixLookAtLH(m_cameraPosition, m_cameraTarget, m_cameraUp);
	
	// Create the projection matrix for 3D rendering.
	m_cameraProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(_fov, _screenAspect, _screenNear, _screenDepth);
}

void Camera::Update(DirectX::XMFLOAT3 _translation)
{
	m_lastPosition = m_position;

	DirectX::XMStoreFloat3(&_translation,
		DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&_translation),
			DirectX::XMMatrixRotationRollPitchYaw(0, m_camYaw, 0) *
			DirectX::XMMatrixScaling(0.05f, 0.05f, 0.05f)
		));

	m_velocity = _translation;

	m_position = { m_position.x + _translation.x, m_position.y + _translation.y, m_position.z + _translation.z };

	// Translate the rotated camera position to the location of the viewer.
	auto lookVector = DirectX::XMVector3Transform(m_defaultForward,
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, m_camYaw, 0.0f));

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	m_cameraPosition = DirectX::XMLoadFloat3(&m_position);
	m_cameraTarget = DirectX::XMVectorAdd(m_cameraPosition, lookVector);

	// Finally create the view matrix from the three updated vectors.
	m_cameraViewMatrix = DirectX::XMMatrixLookAtLH(m_cameraPosition, m_cameraTarget, m_cameraUp);

	// Updates the collider for the camera (move to player when we have one)
	m_collider->UpdateCollider(m_position, { 1,1,1 }, m_velocity);
}

// mostly being used just for the camera for the moment
void Camera::Render(Renderer* renderer, double delta)
{
	for (auto& bullets : m_bulletsFired)
	{
		bullets->SetTexture(renderer, m_bulletTexture);
		bullets->Render(renderer, this, delta);
	}
}

void Camera::Shoot(Texture* bulletTex)
{
	m_bulletTexture = bulletTex;

	Bullet* bullet = new Bullet();
	bullet->Shoot(this);

	m_bulletsFired.emplace_back(bullet);
}


void Camera::RotationCamera(DirectX::XMFLOAT3 _rotation)
{
	// rotation movement
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_camYaw += y;
}



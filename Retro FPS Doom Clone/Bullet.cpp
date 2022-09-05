#include "Bullet.h"
#include "Cube.h"
#include "Texture.h"
#include <iostream>

Bullet::~Bullet()
{
	delete m_bulletCube;
}

void Bullet::Shoot(Camera* camera)
{
	m_bulletCube = new Cube();
	m_timeAlive = 0.0f;

	m_bulletCube->SetRotation({0.0f, 0.0f, 0.0f});
	m_bulletCube->SetPosition(camera->GetPosition());
	m_bulletCube->SetScale({ 0.25, 0.25, 0.25});

	DirectX::XMFLOAT3 cameraForward;
	DirectX::XMStoreFloat3(&cameraForward, camera->GetCameraTarget());

	DirectX::XMFLOAT3 direction = {};
	direction.x = cameraForward.x - camera->GetPosition().x;
	direction.y = cameraForward.y - camera->GetPosition().y;
	direction.z = cameraForward.z - camera->GetPosition().z;

	m_position = direction;
}

void Bullet::Render(Renderer* renderer, Camera* camera, double delta)
{
	if (m_bulletCube != nullptr)
	{
		m_bulletCube->Render(renderer, camera);

		// increase the bullets x & z postion based off the the bullet movement direction.
		float posX = m_bulletCube->GetPosition().x + m_position.x / 4;
		float posZ = m_bulletCube->GetPosition().z + m_position.z / 4;

		// update the bullets position
		m_bulletCube->SetPosition({ posX, m_position.y, posZ });

		// delete this when the bullet has been in the scene longer than neccessary
		m_timeAlive += delta;
		if (m_timeAlive >= m_timeToDestroy)
		{
			// need some kind of call back or something to whoever instigated shooting this?
		}
	}	
}

void Bullet::SetTexture(Renderer* renderer, Texture* texture)
{
	m_bulletCube->SetTexture(renderer, texture);
}


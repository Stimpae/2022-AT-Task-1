#pragma once
#include "pch.h"

class Collider
{
public:
	Collider() = default;
	~Collider() = default;

	// testing various collision methods till i get one that works
	bool CheckAABBCollision(DirectX::XMFLOAT3 _otherPos, DirectX::XMFLOAT3 _otherScale);
	float SweptAABB(DirectX::XMFLOAT3 _staticPos, DirectX::XMFLOAT3 _staticScale, float &normalX, float &normalY);
	void UpdateCollider(DirectX::XMFLOAT3 _objectPos, DirectX::XMFLOAT3 _objScale, DirectX::XMFLOAT3 _velocity);
	DirectX::XMFLOAT2 CalculateDistanceTo(DirectX::XMFLOAT3 _objectPos, DirectX::XMFLOAT3 _objScale);

	bool m_isColliding;

private:

	DirectX::XMFLOAT3 m_objectPosition;
	DirectX::XMFLOAT3 m_objectScale;
	DirectX::XMFLOAT3 m_objectVelocity;
};


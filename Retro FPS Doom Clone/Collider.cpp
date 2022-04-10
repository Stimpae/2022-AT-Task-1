#include "Collider.h"
#include  <limits>
#include <limits.h>
#include <iostream>
#include <DirectXCollision.h>

bool Collider::CheckAABBCollision(DirectX::XMFLOAT3 _otherPos, DirectX::XMFLOAT3 _otherScale)
{
    /*
    float xMin = _otherPos.x - _otherScale.x;
    float xMax = _otherPos.x + _otherScale.x;

    float zMin = _otherPos.z - _otherScale.z;
    float zMax = _otherPos.z + _otherScale.z;

    if (m_objectPosition.x > xMin && m_objectPosition.x < xMax)
    {
        if (m_objectPosition.z > zMin && m_objectPosition.z < zMax)
        {
            return true;
        }
    }
    */

 
   
    return (m_objectPosition.x + m_objectScale.x >= _otherPos.x &&
        _otherPos.x + _otherScale.x >= m_objectPosition.x &&
        m_objectPosition.z + m_objectScale.z >= _otherPos.z &&
        _otherPos.z + _otherScale.z >= m_objectPosition.z);
    
      
 
    
    /*
    return (m_objectPosition.x <= _otherPos.x + _otherScale.x + 2.0f &&
        m_objectPosition.x >= _otherPos.x - _otherScale.x - 2.0f &&
        m_objectPosition.z >= _otherPos.z - _otherScale.z - 2.0f &&
        m_objectPosition.z <= _otherPos.z + _otherScale.z + 2.0f);
        */
    
      
    
    /*
    return (m_objectPosition.x + m_objectVelocity.x < _otherPos.x + _otherScale.x &&
        m_objectPosition.x + m_objectVelocity.x + m_objectScale.x > _otherPos.x &&
        m_objectPosition.z + m_objectVelocity.z < _otherPos.z + _otherScale.z &&
        m_objectPosition.z + +m_objectVelocity.z + m_objectScale.z > _otherPos.z);
        */
     
     
   
}

float Collider::SweptAABB(DirectX::XMFLOAT3 _staticPos, DirectX::XMFLOAT3 _staticScale, float& normalX, float& normalZ)
{
    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;

    // find the distance between the objects on the near and far sides for both x and y 
    if (m_objectVelocity.x > 0.0f)
    {
        xInvEntry = _staticPos.x - (_staticPos.x + _staticScale.x);
        xInvExit = (_staticPos.x + _staticScale.x) - m_objectPosition.x;
    }
    else
    {
        xInvEntry = (_staticPos.x + _staticScale.x) - m_objectPosition.x;
        xInvExit = _staticPos.x - (m_objectPosition.x + m_objectScale.x);
    }

    if (m_objectVelocity.z > 0.0f)
    {
        yInvEntry = _staticPos.z - (_staticPos.z + _staticScale.z);
        yInvExit = (_staticPos.z + _staticScale.z) - m_objectPosition.z;
    }
    else
    {
        yInvEntry = (_staticPos.z + _staticScale.z) - m_objectPosition.z;
        yInvExit = _staticPos.z - (m_objectPosition.z + m_objectScale.z);
    }

    // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero) 
    float xEntry, yEntry;
    float xExit, yExit;

    if (m_objectVelocity.x == 0.0f)
    {
        xEntry = -std::numeric_limits<float>::infinity();
        xExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        xEntry = xInvEntry / m_objectVelocity.x;
        xExit = xInvExit / m_objectVelocity.x;
    }

    if (m_objectVelocity.z == 0.0f)
    {
        yEntry = -std::numeric_limits<float>::infinity();
        yExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        yEntry = yInvEntry / m_objectVelocity.z;
        yExit = yInvExit / m_objectVelocity.z;
    }

    float entryTime = max(xEntry, yEntry);
    float exitTime = min(xExit, yExit);

    if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f)
    {
        normalX = 0.0f;
        normalZ = 0.0f;
        return 1.0f;
    }

    else // if there was a collision 
    {
        // calculate normal of collided surface
        if (xEntry > yEntry)
        {
            if (xInvEntry < 0.0f)
            {
                normalX = 1.0f;
                normalZ = 0.0f;
            }
            else
            {
                normalX = -1.0f;
                normalZ = 0.0f;
            }
        }
        else
        {
            if (yInvEntry < 0.0f)
            {
                normalX = 0.0f;
                normalZ = 1.0f;
            }
            else
            {
                normalX = 0.0f;
                normalZ = -1.0f;
            }
        } 
    }

    return entryTime;
}

void Collider::UpdateCollider(DirectX::XMFLOAT3 _objectPos, DirectX::XMFLOAT3 _objScale, DirectX::XMFLOAT3 _velocity)
{
    m_objectPosition = _objectPos;
    m_objectScale = _objScale;
    m_objectVelocity = _velocity;
}

DirectX::XMFLOAT2 Collider::CalculateDistanceTo(DirectX::XMFLOAT3 _objectPos, DirectX::XMFLOAT3 _objScale)
{
    DirectX::XMFLOAT2 distance = {};

    if (m_objectPosition.x < _objectPos.x)
    {
        distance.x = _objectPos.x - (m_objectPosition.x + m_objectScale.x);
    }
    else if (m_objectPosition.x > _objectPos.x)
    {
        distance.x = m_objectPosition.x - (_objectPos.x + _objScale.x);
    }

    if (m_objectPosition.z < _objectPos.z)
    {
        distance.y = _objectPos.z - (m_objectPosition.z + m_objectScale.z);
    }
    else if (m_objectPosition.z > _objectPos.z)
    {
        distance.y = m_objectPosition.z - (_objectPos.z + _objScale.z);
    }

    return distance;
}

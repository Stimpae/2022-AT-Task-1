#include "Level.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "SystemDefinitions.h"
#include <mutex>
#include <format>

Level::~Level()
{
	delete m_wallTexture;
	delete m_floorTexture;
	delete m_enemyTexture;
}

void Level::Load(const wchar_t* _mapFile)
{
	// textures -> these can be overridden in child classes
	m_wallTexture = m_renderer->CreateTexture(L"brick_texture.jpg");
	m_floorTexture = m_renderer->CreateTexture(L"stone_texture.jpg");
	m_enemyTexture = m_renderer->CreateTexture(L"enemy.png");

	m_camera = std::shared_ptr<Camera>(new Camera());

	float screenHeight = SCREEN_HEIGHT;
	float screenWidth = SCREEN_HEIGHT;
	float aspect = screenWidth / screenHeight;

	// Need to initialise the 2d projection
	m_camera->InitializeProjectionMatrix(0.4f * 3.14f, (float)1024 / 768, 0.01f, 100.0f);
	m_camera->SetPosition(0.0f, 0.5f, 0.0);

	m_mapName = _mapFile;
	m_levelHeight = 4;

	CreateLevelMap();
}

void Level::Update(double _time)
{
	//m_camera->m_lastPosition = m_camera->GetPosition();

	// check for collisionshere
	for (auto& blocks : m_levelBlocks)
	{
		if (blocks->m_active)
		{
			if (m_camera->GetCollider()->CheckAABBCollision(blocks->GetPosition(), blocks->GetScale()))
			{
				auto camPos = m_camera->GetPosition();

				float normalx, normaly;
				
				float collisiontime = m_camera->GetCollider()->SweptAABB(blocks->GetPosition(), blocks->GetScale(), normalx, normaly);
				m_camera->Update({ camPos.x += m_camera->m_velocity.x * collisiontime, 0, camPos.z += m_camera->m_velocity.z * collisiontime });

				float remainingtime = 1.0f - collisiontime;

				if (remainingtime > 1.0f)
				{
					std::cout << "time is remaining" << std::endl;

					float dotprod = (m_camera->m_velocity.x * normaly + m_camera->m_velocity.z * normalx) * remainingtime;
					float moveX = dotprod * normaly;
					float moveZ = dotprod * normalx;
				
					m_camera->Update({ moveX, 0, moveZ });
				}

				// i've tried so many different collision responses and just can't get anything working
				// this is the closet implementation although still very very bugg

	
				/*
				if (normalx < 0)
				{
					std::cout << "x: " << normalx << std::endl;
				}
				if (normalx < 0)
				{
					std::cout << "y: " << normaly << std::endl;
				}
				

				
				*/

				/*
				m_camera->SetPosition(m_camera->m_lastPosition.x, m_camera->m_lastPosition.y, m_camera->m_lastPosition.z);
				auto blockPos = blocks->GetPosition();
				auto blockScale = blocks->GetScale();
				// distance between this position and the other position;
				DirectX::XMFLOAT2 distanceToTarget = m_camera->GetCollider()->CalculateDistanceTo(blockPos, blockScale);
				DirectX::XMFLOAT2 objectVelocity = { m_camera->m_velocity.x, m_camera->m_velocity.z };

				float xAxisTimeToCollide = objectVelocity.x != 0 ? abs(distanceToTarget.x / objectVelocity.x) : 0;
				float yAxisTimeToCollide = objectVelocity.y != 0 ? abs(distanceToTarget.y / objectVelocity.y) : 0;

				float shortestTime = 0;

				float x_min = blockPos.x - blockScale.x;
				float x_max = blockPos.x + blockScale.x;

				float z_min = blockPos.z - blockPos.z;
				float z_max = blockPos.z + blockScale.z;

				
				auto prevPos = m_camera->m_lastPosition;
				auto vector_x = camPos.x - prevPos.x;
				auto vector_z = camPos.z - prevPos.z;

				float moveX = 0;
				float moveZ = 0;

				float deltaX = fmin((prevPos.x - x_min), (x_max - prevPos.x));
				float deltaZ = fmin((prevPos.z - z_min), (z_max - prevPos.z));

				DirectX::XMFLOAT3 objMin = { blockPos.x , blockPos.y, blockPos.z };
				DirectX::XMFLOAT3 objMax = { blockPos.x + blockScale.x, blockPos.y + blockScale.y, blockPos.z + blockScale.z };
				DirectX::XMFLOAT3 thisMin = { camPos.x , camPos.y, camPos.z };
				DirectX::XMFLOAT3 thisMax = { camPos.x + 1, camPos.y + 1, camPos.z + 1 };


				/*
				shortestTime += fmin(abs(xAxisTimeToCollide), abs(yAxisTimeToCollide));

				if (thisMax.x < objMin.x)
				{
					shortestTime += xAxisTimeToCollide;
					moveX = shortestTime * objectVelocity.x;
					std::cout << "we are to the right" << std::endl;
				}
				else if (thisMin.x > objMax.x)
				{
					shortestTime += xAxisTimeToCollide;
					moveX = shortestTime * objectVelocity.x;
					std::cout << "we are to the left" << std::endl;
				}
				else if (thisMax.z < objMin.z)
				{
					shortestTime += yAxisTimeToCollide;
					moveZ = shortestTime * objectVelocity.y;
					std::cout << "we are to the back" << std::endl;
				}
				else if (thisMin.z > objMax.z)
				{
					shortestTime += yAxisTimeToCollide;
					moveZ = shortestTime * objectVelocity.y;
					std::cout << "we are to the front" << std::endl;
				}

				m_camera->TranslateCamera({ moveX, 0, moveZ });
				m_camera->TranslateCamera({ moveX, 0, moveZ });

				/*


				if (deltaX < deltaZ)
				{
					shortestTime = xAxisTimeToCollide;
					moveX = (shortestTime * objectVelocity.x) * -1;
				}
				else
				{
					shortestTime+= yAxisTimeToCollide;
					moveZ = (shortestTime * objectVelocity.y)  * -1;
				}

			

				if (m_camera->m_velocity.x != 0 && m_camera->m_velocity.y > -0.9f && m_camera->m_velocity.y < 0.9f)
				{
					std::cout << "x movement" << std::endl;
					// Colliison on X-axis only
					shortestTime += xAxisTimeToCollide;
						moveX = shortestTime * objectVelocity.x;
				}
				else if (m_camera->m_velocity.x > -0.5f && m_camera->m_velocity.x < 0.5f && m_camera->m_velocity.y != 0)
				{
					std::cout << "y movement" << std::endl;
					// Collision on Y-axis only
					shortestTime += yAxisTimeToCollide;
						moveZ = shortestTime * objectVelocity.y;
				}
				else
				{
		
					// Collision on X and Y axis (eg. slide up against a wall)
					shortestTime = min(abs(xAxisTimeToCollide), abs(yAxisTimeToCollide));
					moveX = shortestTime * objectVelocity.x;
					moveZ = shortestTime * objectVelocity.y;
				}

				m_camera->TranslateCamera({ moveX, 0, moveZ });
				m_camera->TranslateCamera({ moveX, 0, moveZ });


				// get the normal direction of where we are going to collide with the object
				// front, back, left, right


				/*
				*/
			
			}
		}
	}

	
}

void Level::Render()
{
	;

	// draws the map created
	for (int i = 0; i < m_levelBlocks.size(); i++)
	{
		if (m_levelBlocks.at(i)->GetPosition().y == -2 || m_levelBlocks.at(i)->GetPosition().y == m_levelHeight - 2)
		{
			m_levelBlocks.at(i)->SetTexture(m_renderer.get(), m_floorTexture);
			m_levelBlocks.at(i)->Render(m_renderer.get(), m_camera.get());
		}
		else
		{
			m_levelBlocks.at(i)->SetTexture(m_renderer.get(), m_wallTexture);
			m_levelBlocks.at(i)->Render(m_renderer.get(), m_camera.get());
		}
	}

	
	m_renderer->EnabledAlphaBlending(true);
	m_enemy->SetTexture(m_renderer.get(), m_enemyTexture);
	m_enemy->Render(m_renderer.get(), m_camera.get());
	m_renderer->EnabledAlphaBlending(false);
	m_enemy->Update();
}

void Level::OnInputReceived(Input* input, double delta)
{
	m_camera->Render(m_renderer.get(), delta);

		if (input->IsKeyDown(DIK_A))
		{
			m_camera->Update({ -1,0,0 });
		}
		if (input->IsKeyDown(DIK_D))
		{
			m_camera->Update({ 1,0,0 });
		}
		if (input->IsKeyDown(DIK_W)) 
		{
			m_camera->Update({ 0,0,1 });
		}
		if (input->IsKeyDown(DIK_S))
		{
			m_camera->Update({ 0,0,-1 });
		}
		if (input->IsKeyHit(DIK_SPACE))
		{
			m_camera->Shoot(m_floorTexture);
		}


	m_camera->SetRotation(0, input->GetMouseX() * delta / 10.0f, 0);
	m_camera->Update({ 0,0,0 });
	
	input->Update();
	
}

void Level::CreateLevelMap()
{
	std::fstream file(m_mapName);
	std::string line;
	int xPos = 0;
	int zPos = 0;

	// will need to update this for enemys, doors, keys
	while (std::getline(file, line))
	{
		for (auto c : line)
		{
			switch (c)
			{
			case 'S':
			{
				// sets the default start position for this level
				// no break will cause the floors to be built anyway
				m_startPosition = { static_cast<float>(xPos), 0.2f, static_cast<float>(zPos) };
				m_camera->SetPosition(m_startPosition.x, m_startPosition.y, m_startPosition.z);

			}
			case 'E':
			{
				m_enemy = std::make_unique<Billboard>();
				m_enemy->SetPosition({ static_cast<float>(xPos), 0, static_cast<float>(zPos) });
				m_enemy->SetScale({ 2,2,2 });
		
			}
			case '~': // floor
			{
				for (int i = 1; i <= 2; i++)
				{
					Cube* floorCube = new Cube();
					if (i == 2)
					{
						floorCube->SetPosition({ static_cast<float>(xPos), static_cast<float>(m_levelHeight - 2), static_cast<float>(zPos) });
						floorCube->SetScale({ 1,1,1 });
					}
					else
					{
						floorCube->SetPosition({ static_cast<float>(xPos), -2, static_cast<float>(zPos) });
						floorCube->SetScale({ 1,1,1 });
					}

					m_levelBlocks.emplace_back(floorCube);
				}
				break;
			}
			case 'W': // walls  
			{
				for (int i = 0; i <= m_levelHeight; i++)
				{
					Cube* wallCube = new Cube();

					wallCube->m_active = true;
					wallCube->SetPosition({ static_cast<float>(xPos), static_cast<float>(i - 2), static_cast<float>(zPos) });
					wallCube->SetScale({ 1,1,1 });

					m_levelBlocks.emplace_back(wallCube);
				}
				
				break;
			}
			default:
				break;
			}
			xPos++;
		}
		zPos++;
		xPos = 0;
	}
}

void Level::HandleLevelCollision(DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _scale)
{
	// hard code in the scale for the camera as we don't have a player yet.
	auto pos = m_camera->GetPosition();
	auto scale = DirectX::XMFLOAT3(1, 1, 1);

	

	//m_camera->TranslateCamera({ seperation.x, 0, seperation.y });
	

}

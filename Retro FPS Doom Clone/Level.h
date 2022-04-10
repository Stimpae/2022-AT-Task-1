#pragma once
#include "Camera.h"
#include "Renderer.h"
#include "Texture.h"
#include "Input.h"
#include "Cube.h"

#include <memory>
#include "Billboard.h"

class Level
{
public:

	Level(std::shared_ptr<Renderer> _renderer) { m_renderer = _renderer;};
	virtual ~Level();

	virtual void Load(const wchar_t* _mapFile);
	virtual void Update(double _time);
	virtual void Render();
	virtual void OnInputReceived(Input* input, double delta);

	Renderer* GetRenderer() { return m_renderer.get(); };
	Camera* GetCamera() { return m_camera.get(); };

protected:

	Texture* m_wallTexture;
	Texture* m_floorTexture;

private:

	void CreateLevelMap();
	void HandleLevelCollision(DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _scale);

	std::vector<Cube*> m_levelBlocks;

	std::shared_ptr<Renderer> m_renderer;
	std::shared_ptr<Camera> m_camera;

	Texture* m_enemyTexture;

	std::unique_ptr<Billboard> m_enemy;

	DirectX::XMFLOAT3 m_startPosition;
	int m_levelHeight = 4;
	const wchar_t* m_mapName;
};


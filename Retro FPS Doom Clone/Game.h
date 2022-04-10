#pragma once
#include "Renderer.h"
#include "Camera.h"
#include "Cube.h"
#include "Input.h"
#include "Timer.h"
#include "pch.h"
#include "Text.h"
#include "TextureManager.h"
#include "Texture.h"
#include <memory>
#include "Level.h"

class Game
{
public:
	Game();
	~Game();

	bool Initialise(std::shared_ptr<Renderer> _renderer, std::shared_ptr<Timer> _timer);
	virtual void Update(double time);
	virtual void Render();
	virtual void OnInputReceived(Input* input);

private:
	
	Text* m_fpsText;

	Level* m_testLevel;

	std::shared_ptr<Timer> m_timer;
	std::shared_ptr<Renderer> m_renderer;
};


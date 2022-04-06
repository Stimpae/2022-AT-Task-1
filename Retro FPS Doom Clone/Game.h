#pragma once
#include "Renderer.h"
#include <memory>

class Game
{
public:
	Game();
	~Game();

	bool Initialise(std::shared_ptr<Renderer> _renderer);
	virtual void Update();
	virtual void Render();

	Renderer* GetRenderer() { return m_renderer.get(); };

private:

	std::shared_ptr<Renderer> m_renderer;
};


#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Initialise(std::shared_ptr<Renderer> _renderer)
{
	m_renderer = _renderer;
	return true;
}
 
void Game::Update()
{


	// renderer always last?
	m_renderer->Update();
}

void Game::Render()
{



	// renderer always last?
	m_renderer->Render();
}

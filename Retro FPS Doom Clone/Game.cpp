#include "Game.h"
#include "SystemDefinitions.h"
#include <iostream>

Game::Game()
{

}

Game::~Game()
{
	
}

bool Game::Initialise(std::shared_ptr<Renderer> _renderer, std::shared_ptr<Timer> _timer)
{
	m_renderer = _renderer;
	m_timer = _timer;
	
	// Text colour is not working (not sure why)
	m_fpsText = new Text(_renderer, D2D1::ColorF::Azure);
	m_testLevel = new Level(_renderer);

	// this needs to be replace with the current active level from the level manager;
	m_testLevel->Load(L"maptesting.txt");

	return true;
}
 
void Game::Update(double time)
{
	m_renderer->Update();

	// this needs to be replace with the current active level from the level manager;
	m_testLevel->Update(time);
}

void Game::Render()
{
	UINT32 fps = m_timer->GetFramesPerSecond();
	std::wstring fpsText = std::to_wstring(fps) + L" FPS";
	m_fpsText->Render(fpsText, 120.0f, 50.0f);

	m_renderer->Render();

	// this needs to be replace with the current active level from the level manager;
	m_testLevel->Render();
}

void Game::OnInputReceived(Input* input)
{
	// this needs to be replace with the current active level from the level manager;
	m_testLevel->OnInputReceived(input, m_timer->GetDeltaTicksInSeconds());
}

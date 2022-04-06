#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Renderer.h"
#include <memory>

class Framework
{   
public:
	Framework();
	~Framework();

	bool Initialise();
	void Run(std::shared_ptr<Graphics> _graphics, std::shared_ptr<Renderer> _renderer);
	
	HWND GetHWND() { return m_hwnd; };

private:
	bool CreateSystemWindow(const wchar_t* _windowName, int _width, int _height, int _posX, int _posY);

	LPCWSTR m_applicationName{};
	HINSTANCE m_hInstance{};
	HWND m_hwnd{};
};


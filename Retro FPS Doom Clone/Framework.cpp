#include "Framework.h"
#include <iostream>
#include "SystemDefinitions.h"
#include "Game.h"
#include "Input.h"
#include "Timer.h"

using namespace std;
using namespace DirectX;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

Framework::Framework()
{

}

Framework::~Framework()
{
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;
}

bool Framework::Initialise()
{
	// attempt to create the x window 
	if (!CreateSystemWindow(L"Retro FPS Doom Clone", SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_POSX, WINDOW_POSY))
	{
		return false;
	}

	return true;
}

void Framework::Run(std::shared_ptr<Graphics> _graphics, std::shared_ptr<Renderer> _renderer)
{
	MSG msg{};
	ZeroMemory(&msg, sizeof(MSG));

	std::shared_ptr<Timer> timer = std::shared_ptr<Timer>(new Timer());

	// Initialise the input system
	std::shared_ptr<Input> input = std::shared_ptr<Input>(new Input());
	input->Initialise(m_hInstance, GetHWND());

	// Initialise the game 
	std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game());
	if (!game->Initialise(_renderer, timer))
	{
		MessageBox(NULL, L"Failed to create game", L"Error", MB_OK);
	}

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer->Update();

			// start to draw the scene
			_graphics->BeginScene(90, 171, 66, 1.0f);
				
			game->Render();
			game->Update(timer->GetDeltaTicksInSeconds());
			
			input->Update();

			// renderer and update the game
			game->OnInputReceived(input.get());
			// present
			_graphics->EndScene();
		}
	}
}

bool Framework::CreateSystemWindow(const wchar_t* _windowName, int _width, int _height, int _posX, int _posY)
{
	WNDCLASSEX wc{};

	m_applicationName = _windowName;
	m_hInstance = GetModuleHandle(NULL);

	//Setting up WNDCLASSEX object
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra		= NULL;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.cbWndExtra		= NULL;
	wc.hbrBackground	= (HBRUSH)COLOR_WINDOW;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance		= m_hInstance;
	wc.lpszClassName	= m_applicationName;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	// if the class registration fails for some reason then fail the function
	if (!RegisterClassEx(&wc))
	{
		// display failed message
		MessageBox(NULL, L"RegisterClassEX() failed", L"Error", MB_OK);
		return false;
	}

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else 
	{
		screenWidth = _width;
		screenHeight = _height;
	}

	// creating the actual window 
	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, m_applicationName, _windowName,
		WS_CAPTION | WS_SYSMENU, _posX, _posY, screenWidth, screenHeight,
		NULL, NULL, m_hInstance, this);

	// if the window creation fails for some reason then fail the function
	if (!m_hwnd)
	{
		// display failed message
		MessageBox(NULL, L"Window Creation Failed", L"Error", MB_OK);
		PostQuitMessage(0);
		return false;
	}

	// show the window, set it in focus and update
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	UpdateWindow(m_hwnd);

	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		DestroyWindow(hwnd);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

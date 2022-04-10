#pragma once

#include "pch.h"
#include <dinput.h>

// apparently this removes message defaulting (jparedes90)
// https://github.com/jparedes90/DXEngineTutorial/blob/master/
// DXEngineTutorial/Source/Input.cpp
#define DIRECTINPUT_VERSION 0x0800

class Input
{
public:

	Input() = default;
	~Input() = default;

	void Initialise(HINSTANCE hInstance, HWND hwnd);
	bool Update();

	bool IsKeyDown(unsigned int key);
	bool IsKeyHit(unsigned int key);
	LONG GetMouseX();
	LONG GetMouseY();

private:

	const static int s_NumKeys = 256;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	Microsoft::WRL::ComPtr<IDirectInput8> m_directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_keyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_mouse;
	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE m_lastMouseState;

	bool m_keys[s_NumKeys];
	bool m_prevKeys[s_NumKeys];

	int m_screenWidth;
	int m_screenHeight;
	int m_mouseX;
	int m_mouseY;
};


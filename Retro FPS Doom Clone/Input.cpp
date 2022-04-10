#include "Input.h"
#include "ErrorHelper.h"
#include "SystemDefinitions.h"

void Input::Initialise(HINSTANCE hInstance, HWND hwnd)
{
	m_screenWidth = SCREEN_WIDTH;
	m_screenHeight = SCREEN_HEIGHT;

	m_mouseX = 0;
	m_mouseY = 0;

	// Initialise the input interface
	Error::ThrowIfFailed(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_directInput, NULL),
		"Initialise the direct input");

	// Initialise the keyboard interface
	Error::ThrowIfFailed(m_directInput->CreateDevice(GUID_SysKeyboard, 
		&m_keyboard, NULL), " Create keyboard device");

	Error::ThrowIfFailed(m_keyboard->SetDataFormat(&c_dfDIKeyboard), 
		"Set keyboard dataformate");

	Error::ThrowIfFailed(m_keyboard->SetCooperativeLevel(hwnd, 
		DISCL_FOREGROUND | DISCL_EXCLUSIVE), "Set keyboard cooperativelevel");

	Error::ThrowIfFailed(m_keyboard->Acquire(), "Aquire keyboard");

	// Initialise the mouse interface
	Error::ThrowIfFailed(m_directInput->CreateDevice(GUID_SysMouse,
		&m_mouse, NULL), " Create mouse device");

	Error::ThrowIfFailed(m_mouse->SetDataFormat(&c_dfDIMouse),
		"Set mouse dataformat");

	Error::ThrowIfFailed(m_mouse->SetCooperativeLevel(hwnd,
		DISCL_FOREGROUND | DISCL_EXCLUSIVE), "Set mouse cooperativelevel");

	Error::ThrowIfFailed(m_mouse->Acquire(), "Aquire mouse");
}

bool Input::Update()
{
	// Copy previous keys to our current keys.
	memcpy(m_prevKeys, m_keys, sizeof(bool) * s_NumKeys);
	
	bool result;

	// If we cannot read the keyboard inputs the return.
	if (!ReadKeyboard()) 
	{
		return false;
	}

	// If we can not read the mouse input then return.
	if (!ReadMouse())
	{
		return false;
	}

	// If we can access all of the inputs then we can process them.
	ProcessInput();

	return true;
}

bool Input::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}

bool Input::IsKeyHit(unsigned int key)
{
	return m_keys[key] && !m_prevKeys[key];
}

LONG Input::GetMouseX()
{
	return m_lastMouseState.lX;
}

LONG Input::GetMouseY()
{
	return m_lastMouseState.lY;
}

bool Input::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keys), (LPVOID)&m_keys);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	//sometimes mouse doesnt get initialized when putting a breakpoint before it reaches its update
	if (m_mouse == NULL)
	{
		m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
		return false;
	}

	HRESULT result;

	//read mouse device
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Input::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseX += m_mouseState.lY;

	m_lastMouseState = m_mouseState;

	if (m_mouseX < 0) m_mouseX = 0;
	if (m_mouseY < 0) m_mouseY = 0;
	if (m_mouseX > m_screenWidth) m_mouseX = m_screenWidth;
	if (m_mouseY > m_screenHeight) m_mouseY = m_screenHeight;
}

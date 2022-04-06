#pragma once

#include "pch.h"

class Graphics
{
public:
	Graphics();
	~Graphics() = default;

	void CreateDeviceIndependentResources();
	void CreateDeviceResources();
	void CreateWindowDependentResources(HWND _hwnd);

	void InitialiseBackBuffer();
	void ReleaseBackBuffer();
	
	void GoFullScreen();
	void GoWindowed();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	ID3D11Device* GetDevice()					{ return m_d3dDevice.Get(); };
	ID3D11DeviceContext* GetDeviceContext()		{ return m_d3dDeviceContext.Get(); };
	ID3D11RenderTargetView* GetRenderTarget()	{ return m_renderTargetView.Get(); }
	ID3D11DepthStencilView* GetDepthStencil()	{ return m_depthStencilView.Get(); }

private:

	bool m_vsyncEnabled;
	int m_videoCardMemory;
	char* m_videoCardDescription;

	// Direct3D Components
	Microsoft::WRL::ComPtr<ID3D11Device>			m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_swapChain;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_backBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	// Direct2D Components
	Microsoft::WRL::ComPtr<ID2D1Factory3>			m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device2>			m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext2>		m_d2dContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1>			m_d2dTargetBitmap;

	Microsoft::WRL::ComPtr<IDWriteFactory3>			m_dwriteFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory2>		m_wicFactory;

	D3D_FEATURE_LEVEL       m_featureLevel;
	D3D11_TEXTURE2D_DESC    m_bbDesc;
	D3D11_VIEWPORT          m_viewport;
};


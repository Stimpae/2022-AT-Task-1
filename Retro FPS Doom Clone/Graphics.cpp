#include "Graphics.h"
#include "ErrorHelper.h"
#include "SystemDefinitions.h"

// Constructor for the graphics
Graphics::Graphics()
{
    // create the independent and dependent device resources
    CreateDeviceIndependentResources();
    CreateDeviceResources();
   
}

// Configures device resources that don't depend on the 3D device (2d)
void Graphics::CreateDeviceIndependentResources()
{
    //Initialize Direct2D resources.
    D2D1_FACTORY_OPTIONS options{};
 
    // Initialize the Direct2D Factory.
    Error::ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,__uuidof(ID2D1Factory3)
        ,&options,&m_d2dFactory), "Creating D2D Factory");

    // Initialize the DirectWrite Factory.
    Error::ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory3),
        &m_dwriteFactory), "Creating DWrite Factory");

    // Initialize the Windows Imaging Component (WIC) Factory.
    // Not sure that i actually need this if i'm using the DirectXTex?
    Error::ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory2,nullptr,
        CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&m_wicFactory)), "Creating WIC Imaging Factory");

   
}

// Create the DirectX device and device context;
void Graphics::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_9_1,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_11_1
    };

    // This flag adds support for surfaces with a color-channel ordering different
    // from the API default. It is required for compatibility with Direct2D.
    UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    // Create the Direct3D 11 API device object and a corresponding context.
    Microsoft::WRL::ComPtr<ID3D11Device>        device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

    hr = D3D11CreateDevice(
        nullptr,                    
        D3D_DRIVER_TYPE_HARDWARE,   
        0,                          
        deviceFlags,                
        levels,                     
        ARRAYSIZE(levels),          
        D3D11_SDK_VERSION,          
        &device,                   
        &m_featureLevel,            
        &context                    
    );

    // Store pointers to the Direct3D 11.1 API device and immediate context.
    device.As(&m_d3dDevice);
    context.As(&m_d3dDeviceContext);

    // Create the Direct2D device object and a corresponding context.
    Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
    m_d3dDevice.As(&dxgiDevice);
     
    m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice);
    m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext);
}

// Create the swap chain and backbuffer (viewport, stencil buffer)
void Graphics::CreateWindowDependentResources(HWND _hwnd)
{
    HRESULT hr = S_OK;

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.Windowed = TRUE; // Sets the initial state of full-screen mode.
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;      //multisampling setting
    desc.SampleDesc.Quality = 0;    //vendor-specific flag
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.OutputWindow = _hwnd;

    // Create the DXGI device object to use in other factories, such as Direct2D.
    Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
    m_d3dDevice.As(&dxgiDevice);

    // Create the adapter and factory 
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;

    Error::ThrowIfFailed(dxgiDevice->GetAdapter(&adapter), "Get Device Adapter");
    Error::ThrowIfFailed(adapter->GetParent(IID_PPV_ARGS(&factory)), "Get Adapter Parent");

    // Create the swap chain
    Error::ThrowIfFailed(factory->CreateSwapChain(m_d3dDevice.Get(),&desc,&m_swapChain), 
        "Create The Swapchain");

    // Initialise the back buffer, stencil buffer, and viewport.
    InitialiseBackBuffer();
}

void Graphics::InitialiseBackBuffer()
{
    HRESULT hr = S_OK;

    // Gets buffer from swapchain
    Error::ThrowIfFailed(m_swapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&m_backBuffer), 
        "Get Backbuffer");

    // Creates the render target view.
    Error::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(m_backBuffer.Get(),nullptr,
        m_renderTargetView.GetAddressOf()), "Create RenderTargetView");

    m_backBuffer->GetDesc(&m_bbDesc);
    m_backBuffer->Release();
    // Outline the Depth Stencil description.
    D3D11_DEPTH_STENCIL_DESC stencilDesc = {};
    stencilDesc.DepthEnable = TRUE;
    stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    // create the depth stencil state
    Error::ThrowIfFailed(m_d3dDevice->CreateDepthStencilState(&stencilDesc, &m_depthStencilState), 
        "Creating the depth stencil state");

    // Create a depth-stencil view for use with 3D rendering if needed.
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = 1024;
    depthStencilDesc.Height = 768;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    // Create the depth stencil texture
    Error::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc,nullptr,&m_depthStencilBuffer
    ), "Create the depth stencil buffer");

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Flags = 0;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view (zbuffer)
    Error::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(),&depthStencilViewDesc,&m_depthStencilView
    )," Create the depth stencil view");

    D3D11_BLEND_DESC blendStateDesc = {};

    blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
    blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

    Error::ThrowIfFailed(m_d3dDevice->CreateBlendState(&blendStateDesc, &m_alphaEnabledBlendState), "Create alpha blend state");

    blendStateDesc.RenderTarget[0].BlendEnable = false;
    
    Error::ThrowIfFailed(m_d3dDevice->CreateBlendState(&blendStateDesc, &m_alphaDisabledBlendState), "Create alpha blend state");

    // Set the 3D rendering viewport to target the entire window.
    ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
    m_viewport.Height = (float)SCREEN_HEIGHT;
    m_viewport.Width = (float)SCREEN_WIDTH;
    m_viewport.MinDepth = 0;
    m_viewport.MaxDepth = 1;

    // Set the viewport within the device context
    m_d3dDeviceContext->RSSetViewports(1,&m_viewport);

    // 2D
    // Create a Direct2D target bitmap associated with the
    // swap chain back buffer and set it as the current target.
    D2D1_BITMAP_PROPERTIES1 bitmapProperties =
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            -1.0f,
            -1.0f
        );

    Microsoft::WRL::ComPtr<IDXGISurface2> dxgiBackBuffer;

    Error::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),"Get swapchain 2D backbuffer");
    Error::ThrowIfFailed(m_d2dContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.Get(),&bitmapProperties,
    &m_d2dTargetBitmap),"Create bitmap from dxgisurface");

    // Set the target bitmap as swapchain target
    m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
 
    m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

// to do
void Graphics::ReleaseBackBuffer()
{
}

// to do
void Graphics::GoFullScreen()
{
}

// to do
void Graphics::GoWindowed()
{
}


void Graphics::BeginScene(float r, float g, float b, float a)
{
    // Assigns a new background colour for the scene
    const float backgroudnColour[] = { r / 255, g / 255, b / 255, a };

    // Clears the render target view and assigns new colour
    m_d3dDeviceContext->ClearRenderTargetView(m_renderTargetView.Get(),backgroudnColour);

    // Clears the stencil view
    m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView.Get(),D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f,0);

    // Sets the depth stencil state.
    m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

    // Set the render target.
    m_d3dDeviceContext->OMSetRenderTargets(1,m_renderTargetView.GetAddressOf(),m_depthStencilView.Get());
}

void Graphics::EndScene()
{
    // Presents the current view to screen
    m_swapChain->Present(1, 0);
}

void Graphics::EnableAlphaBlending(bool enabled)
{
    float blendFactor[4];
    blendFactor[0] = 0.0f;
    blendFactor[0] = 0.0f;
    blendFactor[0] = 0.0f;
    blendFactor[0] = 0.0f;

    if (enabled)
    {
        m_d3dDeviceContext->OMSetBlendState(m_alphaEnabledBlendState.Get(), blendFactor, 0xffffffff);
    }
    else
    {
        m_d3dDeviceContext->OMSetBlendState(m_alphaDisabledBlendState.Get(), blendFactor, 0xffffffff);
    }
}



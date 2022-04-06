#include "Renderer.h"
#include "ErrorHelper.h"
#include "pch.h"

#include <ppltasks.h>
#include <cmath>
#include <iostream>

using namespace DirectX;

Renderer::Renderer(std::shared_ptr<Graphics> _graphics)
{
	m_graphics = _graphics;
	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
}

void Renderer::CreateDeviceDependentResources()
{
	// Pulled from the Microsoft directX11 documentation.
	// Contains some refactoring from the visual studio dx11 example as well 
	// as my own implementation.

	// create the vertex shader here; 
	auto CreateVSTask = Concurrency::create_task(
		[this]()
		{
			void* shader_byte_code = nullptr;
			size_t size_shader = 0;

			CompileShaderFromFile(L"VertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
			
			Error::ThrowIfFailed(m_graphics->GetDevice()->CreateVertexShader(shader_byte_code,
				size_shader, nullptr, &m_vertexShader), "Create Vertex Shader");

			static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			Error::ThrowIfFailed(m_graphics->GetDevice()->CreateInputLayout(vertexDesc,ARRAYSIZE(vertexDesc),
					shader_byte_code,size_shader,&m_inputLayout), "Create Input Layout");
		}
	);

	// create pixel shader here;
	auto CreatePSTask = CreateVSTask.then(
		[this]()
		{
			void* shader_byte_code = nullptr;
			size_t size_shader = 0;

			CompileShaderFromFile(L"PixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);

			Error::ThrowIfFailed(m_graphics->GetDevice()->CreatePixelShader(shader_byte_code,
				size_shader,nullptr,&m_pixelShader),"Create Pixel Shader");

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
			Error::ThrowIfFailed(m_graphics->GetDevice()->CreateBuffer(&constantBufferDesc,nullptr,
					&m_constantBuffer),"Create Constant Buffer");
		}
	);

	// Load the geometry for the spinning cube.
	auto CreateCubeTask = CreatePSTask.then(
		[this]()
		{
			// Load mesh vertices. Each vertex has a position and a color.
			static const VertexPositionColor cubeVertices[] =
			{
				{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
				{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
				{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
				{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
				{XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
				{XMFLOAT3(0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
				{XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
				{XMFLOAT3(0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			};

			D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
			vertexBufferData.pSysMem = cubeVertices;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
			
			Error::ThrowIfFailed(m_graphics->GetDevice()->CreateBuffer(&vertexBufferDesc,&vertexBufferData,
					&m_vertexBuffer),"Create Vertex Buffer");

			// Load mesh indices. Each trio of indices represents
			// a triangle to be rendered on the screen.
			// For example: 0,2,1 means that the vertices with indexes
			// 0, 2 and 1 from the vertex buffer compose the 
			// first triangle of this mesh.
			static const unsigned short cubeIndices[] =
			{
				0,2,1, // -x
				1,2,3,

				4,5,6, // +x
				5,7,6,

				0,1,5, // -y
				0,5,4,

				2,6,7, // +y
				2,7,3,

				0,4,6, // -z
				0,6,2,

				1,3,7, // +z
				1,7,5,
			};

			m_indexCount = ARRAYSIZE(cubeIndices);

			D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
			indexBufferData.pSysMem = cubeIndices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
			Error::ThrowIfFailed(m_graphics->GetDevice()->CreateBuffer(&indexBufferDesc,&indexBufferData,
					&m_indexBuffer),"Create Index Buffer");
		}
	);

	CreateCubeTask.then([this]() {
		m_loadingComplete = true;
		});
}

// This can eventually be replaced by the camera 
void Renderer::CreateWindowSizeDependentResources()
{
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

	DirectX::XMStoreFloat4x4(&m_constantBufferData.view,DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookAtRH(eye,at,up)));

	DirectX::XMStoreFloat4x4(&m_constantBufferData.projection,DirectX::XMMatrixTranspose(
		DirectX::XMMatrixPerspectiveFovRH(2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) / 0.1)
			,1,0.01f,100.0f)));
}

void Renderer::Update()
{
	
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(2);
		double totalRotation = 2 * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));

}

void Renderer::Render()
{
	// think we need a reference to the camera for the renderer to render things properly

	// Will only render the geomtry once the 
	if (!m_loadingComplete)
	{
		return;
	}

	auto deviceContext = m_graphics->GetDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	deviceContext->UpdateSubresource(m_constantBuffer.Get(),0,NULL,&m_constantBufferData,0,0);

	// Set up the IA stage by setting the input topology and layout.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0,1,m_vertexBuffer.GetAddressOf(),&stride,&offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(),DXGI_FORMAT_R16_UINT,0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->IASetInputLayout(m_inputLayout.Get());

	// Set up the vertex shader
	deviceContext->VSSetShader(m_vertexShader.Get(),nullptr,0);

	// Set the constant buffer to the device
	deviceContext->VSSetConstantBuffers(0,1,m_constantBuffer.GetAddressOf());

	// Set up the pixel shader
	deviceContext->PSSetShader(m_pixelShader.Get(),nullptr,0);

	// Draw the object (assign this individualy to each object instead of like this?
	deviceContext->DrawIndexed(m_indexCount,0,0);
}

void Renderer::CompileShaderFromFile(const wchar_t* _file, const char* _entryPoint, const char* _target, void** _shaderByteCode, size_t* _byteCodeSize)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(_file, nullptr, nullptr, _entryPoint, _target, 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob)
		{
			error_blob->Release();
		}
	}

	*_shaderByteCode = m_blob->GetBufferPointer();
	*_byteCodeSize = m_blob->GetBufferSize();
}

void Renderer::CompileShaderRelease()
{
	if (m_blob)
	{
		m_blob->Release();
	}
}



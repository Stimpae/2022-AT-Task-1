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
	m_textureManager = std::make_unique<TextureManager>();
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

			CompileVertexShaderFromFile(L"VertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
			
			Error::ThrowIfFailed(m_graphics->GetDevice()->CreateVertexShader(shader_byte_code,
				size_shader, nullptr, &m_vertexShader), "Create Vertex Shader");

			static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT , 0 , 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

			CompilePixelShaderFromFile(L"PixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
		
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
			DirectX::XMFLOAT3 vertexPositions[] =
			{
				{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f)},
				{DirectX::XMFLOAT3(-0.5f,0.5f,-0.5f)},
				{DirectX::XMFLOAT3(0.5f,0.5f,-0.5f)},
				{DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f)},

				{DirectX::XMFLOAT3(0.5f,-0.5f,0.5f)},
				{DirectX::XMFLOAT3(0.5f,0.5f,0.5f)},
				{DirectX::XMFLOAT3(-0.5f,0.5f,0.5f)},
				{DirectX::XMFLOAT3(-0.5f,-0.5f,0.5f)}
			};

			DirectX::XMFLOAT2 textureCoordinates[] =
			{
				{DirectX::XMFLOAT2(0.0f, 0.0f)},
				{DirectX::XMFLOAT2(0.0f, 1.0f)},
				{DirectX::XMFLOAT2(1.0f, 0.0f)},
				{DirectX::XMFLOAT2(1.0f, 1.0f)},
			};

			// Load mesh vertices. Each vertex has a position and a color.
			static const VertexPositionTexture cubeVertices[] =
			{
				{vertexPositions[0], textureCoordinates[1]},
				{vertexPositions[1], textureCoordinates[0]},
				{vertexPositions[2], textureCoordinates[2]},
				{vertexPositions[3], textureCoordinates[3]},

				{vertexPositions[4], textureCoordinates[1]},
				{vertexPositions[5], textureCoordinates[0]},
				{vertexPositions[6], textureCoordinates[2]},
				{vertexPositions[7], textureCoordinates[3]},

				{vertexPositions[1], textureCoordinates[1]},
				{vertexPositions[6], textureCoordinates[0]},
				{vertexPositions[5], textureCoordinates[2]},
				{vertexPositions[2], textureCoordinates[3]},

				{vertexPositions[7], textureCoordinates[1]},
				{vertexPositions[0], textureCoordinates[0]},
				{vertexPositions[3], textureCoordinates[2]},
				{vertexPositions[4], textureCoordinates[3]},

				{vertexPositions[3], textureCoordinates[1]},
				{vertexPositions[2], textureCoordinates[0]},
				{vertexPositions[5], textureCoordinates[2]},
				{vertexPositions[4], textureCoordinates[3]},

				{vertexPositions[7], textureCoordinates[1]},
				{vertexPositions[6], textureCoordinates[0]},
				{vertexPositions[1], textureCoordinates[2]},
				{vertexPositions[0], textureCoordinates[3]},
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
				0,1,2,
				2,3,0,

				4,5,6,
				6,7,4,

				8,9,10,
				10,11,8,

				12,13,14,
				14,15,12,

				16,17,18,
				18,19,16,

				20,21,22,
				22,23,20
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

void Renderer::Update()
{

}

void Renderer::Render()
{
	if (!m_loadingComplete)
	{
		return;
	}

	auto deviceContext = m_graphics->GetDeviceContext();

	// Set up the IA stage by setting the input topology and layout.
	UINT stride = sizeof(VertexPositionTexture);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0,1,m_vertexBuffer.GetAddressOf(),&stride,&offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(),DXGI_FORMAT_R16_UINT,0);

	deviceContext->IASetInputLayout(m_inputLayout.Get());

	// Set up the vertex shader
	deviceContext->VSSetShader(m_vertexShader.Get(),nullptr,0);

	// Set the constant buffer to the device
	deviceContext->VSSetConstantBuffers(0,1,m_constantBuffer.GetAddressOf());

	// Set up the pixel shader
	deviceContext->PSSetShader(m_pixelShader.Get(),nullptr,0);
}

void Renderer::UpdateConstantBuffer(ModelViewProjectionConstantBuffer buffer, unsigned int _indexCount)
{
	if (!m_loadingComplete)
	{
		return;
	}

	auto deviceContext = m_graphics->GetDeviceContext();
	
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, NULL, &buffer, 0, 0);
	
	// Draw the object (assign this individualy to each object instead of like this?
	deviceContext->DrawIndexed(_indexCount, 0, 0);
}

void Renderer::UpdatePixelShaderResources(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resources)
{
	auto deviceContext = m_graphics->GetDeviceContext();
	deviceContext->PSSetShaderResources(0, 1, resources.GetAddressOf());
}

Texture* Renderer::CreateTexture(const wchar_t* _filePath)
{
	return m_textureManager->CreateTextureFromFIle(_filePath, m_graphics.get());
}

void Renderer::EnabledAlphaBlending(bool enabled)
{
	m_graphics->EnableAlphaBlending(enabled);
}

void Renderer::CompilePixelShaderFromFile(const wchar_t* _file, const char* _entryPoint, const char* _target, void** _shaderByteCode, size_t* _byteCodeSize)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(_file, nullptr, nullptr, _entryPoint, _target, 0, 0, &m_pixelBlob, &error_blob)))
	{
		if (error_blob)
		{
			error_blob->Release();
		}
	}

	*_shaderByteCode = m_pixelBlob->GetBufferPointer();
	*_byteCodeSize = m_pixelBlob->GetBufferSize();
}

void Renderer::CompileVertexShaderFromFile(const wchar_t* _file, const char* _entryPoint, const char* _target, void** _shaderByteCode, size_t* _byteCodeSize)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(_file, nullptr, nullptr, _entryPoint, _target, 0, 0, &m_vertexBlob, &error_blob)))
	{
		if (error_blob)
		{
			error_blob->Release();
		}
	}

	*_shaderByteCode = m_vertexBlob->GetBufferPointer();
	*_byteCodeSize = m_vertexBlob->GetBufferSize();
}




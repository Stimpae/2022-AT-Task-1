#pragma once
#include "pch.h"
#include "ShaderStructures.h"
#include "Graphics.h"
#include <memory>

class Renderer
{
public:
	Renderer(std::shared_ptr<Graphics> _graphics);
    ~Renderer() = default;

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    void Update();
    void Render();

private:

    void CompileShaderFromFile(const wchar_t* _file, const char* _entryPoint, const char* _target ,void** _shaderByteCode, size_t* _byteCodeSize);
    void CompileShaderRelease();

    std::shared_ptr<Graphics> m_graphics;

    unsigned int  m_indexCount;
    bool m_loadingComplete = false;

    ModelViewProjectionConstantBuffer m_constantBufferData{};
 
    ID3DBlob* m_blob;
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayoutExtended;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_constantBuffer;
};


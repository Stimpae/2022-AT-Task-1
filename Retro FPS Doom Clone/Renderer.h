#pragma once
#include "pch.h"
#include "ShaderStructures.h"
#include "Graphics.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Texture.h"
#include <memory>

class Renderer
{
public:
	Renderer(std::shared_ptr<Graphics> _graphics);
    ~Renderer() = default;

    void CreateDeviceDependentResources();
    void Update();
    void Render();

    void UpdateConstantBuffer(ModelViewProjectionConstantBuffer buffer, unsigned int _indexCount);
    void UpdatePixelShaderResources(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resources);

    Texture* CreateTexture(const wchar_t* _filePath);
    void EnabledAlphaBlending(bool enabled);

    // Just being a bit lazy here.
    std::shared_ptr<Graphics> m_graphics;

private:

    void CompilePixelShaderFromFile(const wchar_t* _file, const char* _entryPoint, const char* _target ,void** _shaderByteCode, size_t* _byteCodeSize);
    void CompileVertexShaderFromFile(const wchar_t* _file, const char* _entryPoint, const char* _target, void** _shaderByteCode, size_t* _byteCodeSize);

    
    std::unique_ptr<TextureManager> m_textureManager;

    unsigned int  m_indexCount;
    bool m_loadingComplete = false;

    ModelViewProjectionConstantBuffer m_constantBufferData{};
 
    ID3DBlob* m_vertexBlob;
    ID3DBlob* m_pixelBlob;
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayoutExtended;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_constantBuffer;
};


#pragma once

#include "Graphics.h"
#include "Renderer.h"
#include "pch.h"
#include <string>
#include <memory>

class Text
{
public:
	Text(std::shared_ptr<Renderer> _renderer, D2D1::ColorF colour);
	~Text();

	void Update(double time);
	void Render(std::wstring text, float xPos, float yPos);

private:
	void CreateDeviceDependentResources(D2D1::ColorF colour);

	std::shared_ptr<Renderer> m_renderer;
	std::shared_ptr<Graphics> m_graphics;

	std::wstring                                    m_text;
	DWRITE_TEXT_METRICS	                            m_textMetrics;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_whiteBrush;
	Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
	Microsoft::WRL::ComPtr<IDWriteTextLayout3>      m_textLayout;
	Microsoft::WRL::ComPtr<IDWriteTextFormat2>      m_textFormat;
};


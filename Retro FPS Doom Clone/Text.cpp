#include "Text.h"
#include "ErrorHelper.h"
#include "SystemDefinitions.h"

Text::Text(std::shared_ptr<Renderer> _renderer, D2D1::ColorF colour)
{
	m_renderer = _renderer;
	m_graphics = _renderer->m_graphics;

	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

	// Create device independent resources
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
	Error::ThrowIfFailed(m_graphics->GetDWriteFactory()->CreateTextFormat(L"Verdana",
		nullptr,DWRITE_FONT_WEIGHT_LIGHT,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,
		32.0f,L"en-US",&textFormat),"Create text format");

	Error::ThrowIfFailed(textFormat.As(&m_textFormat), "Assign text format");
	Error::ThrowIfFailed(m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
	, "Set paragraph alignment");
	Error::ThrowIfFailed(m_graphics->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
	, "Create state block");

	CreateDeviceDependentResources(colour);
}

Text::~Text()
{
	m_whiteBrush.Reset();
}

void Text::Update(double time)
{
}

void Text::Render(std::wstring text, float xPos, float yPos)
{
	m_text = text;

	Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
	Error::ThrowIfFailed(m_graphics->GetDWriteFactory()->CreateTextLayout(
			m_text.c_str(),(UINT32)m_text.length(),m_textFormat.Get(), xPos,
		yPos, &textLayout), "Create text layout");

	Error::ThrowIfFailed(textLayout.As(&m_textLayout), "Assign text layout");
	Error::ThrowIfFailed(m_textLayout->GetMetrics(&m_textMetrics), "Get metrics");

	ID2D1DeviceContext* context = m_graphics->GetD2DDeviceContext();

	context->SaveDrawingState(m_stateBlock.Get());

	context->BeginDraw();

	context->SetTransform(D2D1::Matrix3x2F::Identity());

	context->Clear(D2D1::ColorF(D2D1::ColorF::Aqua));

	Error::ThrowIfFailed(m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING)
	, "Set text aligment");

	context->DrawTextLayout(D2D1::Point2F(0.f, 0.f),m_textLayout.Get(),m_whiteBrush.Get());

	context->RestoreDrawingState(m_stateBlock.Get());

	context->EndDraw();
}

void Text::CreateDeviceDependentResources(D2D1::ColorF colour)
{
	Error::ThrowIfFailed(m_graphics->GetD2DDeviceContext()->
		CreateSolidColorBrush(colour,
			&m_whiteBrush),"Create Color Brush");
}


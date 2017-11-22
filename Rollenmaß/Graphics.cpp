#include "Graphics.h"
#include "stdafx.h"

using namespace std;

static long double PI = 3.141592653589793238463;

Graphics::Graphics() {
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
}


Graphics::~Graphics() {
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
	if (writefactory) writefactory->Release();
	if (tf)tf->Release();
}

bool Graphics::Init(HWND windowHandle) {

	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);

	if (res != S_OK) { return false; }
	RECT rect;
	GetClientRect(windowHandle, &rect);

	factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget);

	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&writefactory)
	);

	if (res != S_OK) return false;

	res = renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);


	res = writefactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"en-us", &tf);
	if (res != S_OK) return false;
	tf->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	return true;

}

void Graphics::ClearScreen(float r, float g, float b) {
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::SetRotationsWinkel(float winkel) {
	rotationswinkel=winkel;
}

void Graphics::Rotieren(float &x,float &y) {

	x = x * cos((rotationswinkel*PI) / 180.0) -y * (sin((rotationswinkel*PI) / 180.0));
	y = x * sin((rotationswinkel*PI) / 180.0) +y * (cos((rotationswinkel*PI) / 180.0));
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a) {

	Rotieren(x, y);
	radius *= (vergroesserung/2);

	x *= vergroesserung;
	y *= vergroesserung;

	x += centerPosX;
	y += centerPosY;
	brush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush);
}

void Graphics::DrawOval(float x, float y, float radius1, float radius2, float r, float g, float b, float a) {
	Rotieren(x, y);
	radius1 *= (vergroesserung / 2);
	radius2 *= (vergroesserung / 2);

	x *= vergroesserung;
	y *= vergroesserung;
	x += centerPosX;
	y += centerPosY;
	brush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius1, radius2), brush);

}

void Graphics::DrawRect(float x, float y,  float width, float height, float r, float g, float b, float a) {
	/*width *= (vergroesserung / 2);
	height *= (vergroesserung / 2);
	x *= vergroesserung;
	y *= vergroesserung;
	x += centerPosX;
	y += centerPosY;*/

	brush->SetColor(D2D1::ColorF(r, g, b, a));
	D2D1_RECT_F* rect;
	rect = new D2D1_RECT_F();
	rect->bottom = y + height;
	rect->top = y;
	rect->left = x;
	rect->right = x + width;
	renderTarget->FillRectangle(rect, brush);
	brush->SetColor(D2D1::ColorF(0.2f, 0.5f, 0.6f, 10.0f));
	renderTarget->DrawRectangle(rect, brush, 1.0f, 0);
	delete rect;
}


void Graphics::DrawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a) {
	Rotieren(x1, y1);
	Rotieren(x2, y2);
	x1 *= vergroesserung;
	y1 *= vergroesserung;
	x2 *= vergroesserung;
	y2 *= vergroesserung;

	x1 += centerPosX;
	y1 += centerPosY;
	x2 += centerPosX;
	y2 += centerPosY;

	brush->SetColor(D2D1::ColorF(r, g, b, a));
	D2D_POINT_2F point1;
	point1.x = x1;
	point1.y = y1;
	D2D_POINT_2F point2;
	point2.x = x2;
	point2.y = y2;
	renderTarget->DrawLine(point1, point2, brush, 1.0f, 0);
}


void Graphics::DrawLineRelativ(float x, float y, float u, float v, float r, float g, float b, float a, bool normale) {
	Rotieren(x, y);
	//Rotieren(u, v);
	x *= vergroesserung;
	y *= vergroesserung;

	if (normale) {
		u *= vergroesserungNormale;
		v *= vergroesserungNormale;
	}
	else {
		u *= 5* vergroesserung;
		v *= 5* vergroesserung;
	}

	x += centerPosX;
	y += centerPosY;

	u += x;
	v += y;

	brush->SetColor(D2D1::ColorF(r, g, b, a));
	D2D_POINT_2F point1;
	point1.x = x;
	point1.y = y;
	D2D_POINT_2F point2;
	point2.x = u;
	point2.y = v;
	renderTarget->DrawLine(point1, point2, brush, 1.0f, 0);
}

void Graphics::DrawTextS(float x, float y, float width, float height, std::wstring* str, bool rotieren) {
	if (rotieren) {
		Rotieren(x, y);
	}
	/*x *= vergroesserung;
	y *= vergroesserung;

	x += centerPosX;
	y += centerPosY;*/

	brush->SetColor(D2D1::ColorF(0.05f, 0.05f, 0.05f, 10.0f));

	D2D1_RECT_F* rect;
	rect = new D2D1_RECT_F();
	rect->bottom = y+ height;
	rect->top = y;
	rect->left = x;
	rect->right = x + width;

	renderTarget->DrawTextW(str->c_str(), wcslen(str->c_str()), tf, rect, brush, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);

	delete rect;
}

void Graphics::DrawTextS(float x, float y, float width, float height, std::wstring str,bool rotieren) {
	if (rotieren) {
		Rotieren(x, y);
	}

	float x2 = (x - 2) * vergroesserung;
	float y2 = y * vergroesserung;

	x2 = x2 + centerPosX;
	y2 = y2 + centerPosY;

	x *= vergroesserung;
	y *= vergroesserung;

	x += centerPosX;
	y += centerPosY;


	

	D2D1_RECT_F* rect2;
	rect2 = new D2D1_RECT_F();
	rect2->bottom = y2 + height;
	rect2->top = y2;
	rect2->left = x2;
	rect2->right = x2 + width;
	brush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.1f, 10.0f));
	renderTarget->DrawRectangle(rect2, brush, 10.0f, 0);

	D2D1_RECT_F* rect;
	rect = new D2D1_RECT_F();
	rect->bottom = y + height;
	rect->top = y;
	rect->left = x;
	rect->right = x + width;
	brush->SetColor(D2D1::ColorF(0.8f, 0.8f, 0.8f, 10.0f));
	renderTarget->DrawTextW(str.c_str(), wcslen(str.c_str()), tf, rect, brush, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);

	delete rect;
}

static mutex lockGraph;

void Graphics::DrawPoint(float x, float y, float r, float g, float b, float a) {
	Rotieren(x, y);

	x *= vergroesserung;
	y *= vergroesserung;
	x += centerPosX;
	y += centerPosY;

	try {
		lockGraph.lock();
		brush->SetColor(D2D1::ColorF(r, g, b, a));
		renderTarget->DrawLine(D2D1::Point2F(x, y), D2D1::Point2F(x + 1, y), brush);
		lockGraph.unlock();
	}
	catch (exception e) {
		return;
	}
}

void Graphics::DrawPoint(float x, float y, float z, float r, float g, float b, float a) {
	Rotieren(x, y);
	x *= vergroesserung;
	y *= vergroesserung;
	x += centerPosX;
	y += centerPosY;
	brush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->DrawLine(D2D1::Point2F(x, y), D2D1::Point2F(x + 1, y), brush);

}

void Graphics::setCenterPosition(float x, float y) {
	centerPosX = x;
	centerPosY = y;
}

void Graphics::moveCenterPosition(float x, float y) {
	centerPosX += x;
	centerPosY += y;
}

void Graphics::setVergroesserung(float faktor) {
	vergroesserung = faktor;
}

void Graphics::setVergroesserungNormale(float faktor) {
	vergroesserungNormale = faktor;
}

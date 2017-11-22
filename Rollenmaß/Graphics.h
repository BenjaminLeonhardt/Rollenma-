#pragma once
#include "stdafx.h"
#include <d2d1.h>
#include <iostream>
#include <exception>
#include <Dwrite.h>

class Graphics {
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1SolidColorBrush* brush;
	IDWriteFactory* writefactory;
	IDWriteTextFormat* tf;
	float centerPosX;
	float centerPosY;
	float vergroesserung;
	float vergroesserungNormale;
	float rotationswinkel;

public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);
	void BeginDraw() { renderTarget->BeginDraw(); }
	void EndDraw() { renderTarget->EndDraw(); }

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawOval(float x, float y, float radius1, float radius2, float r, float g, float b, float a);
	void DrawRect(float x, float y, float height, float width, float r, float g, float b, float a);
	void DrawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a);
	void DrawLineRelativ(float x1, float y1, float x2, float y2, float r, float g, float b, float a, bool normale);
	void DrawTextS(float x, float y, float width, float height, std::wstring* string, bool rotieren);
	void DrawTextS(float x, float y, float width, float height, std::wstring string, bool rotieren);
	void DrawPoint(float x, float y, float r, float g, float b, float a);
	void DrawPoint(float x, float y, float z, float r, float g, float b, float a);
	void setCenterPosition(float x, float y);
	void moveCenterPosition(float x, float y);
	void setVergroesserung(float faktor);
	void setVergroesserungNormale(float faktor);
	void SetRotationsWinkel(float winkel);
	void Rotieren(float &x, float &y);
};


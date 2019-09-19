
#pragma once

#include <windows.h>
#include "ScreenPoint.h"
#include "LinearColor.h"

class WindowsGDI
{
public:
	WindowsGDI() { };
	~WindowsGDI() { ReleaseGDI(); }

public:
	void InitializeGDI(const ScreenPoint& InDisplaySize, const bool InbSRGBColorSpace);
	void ReleaseGDI();

	void FillBuffer();
	Color32* CopyBuffer(Color32* InDst, Color32* InSrc, int InCount);
	void SetColor(const LinearColor& InColor);
	LinearColor GetPixel(const ScreenPoint& InPos);
	void SetPixel(const ScreenPoint& InPos);
	void SetPixelWithoutAlpha(const ScreenPoint& InPos, const LinearColor& InColor);
	void SetPixelWithAlpha(const ScreenPoint& InPos, const LinearColor& InColor);

	void CreateDepthBuffer();
	void ClearDepthBuffer();
	float GetDepthBufferValue(const ScreenPoint& InPos) const;
	void SetDepthBufferValue(const ScreenPoint& InPos, float InDepthValue);

	Color32* GetScreenBuffer() const;

	void SwapBuffer();

protected:
	bool bSRGBColorSpace = false;
	bool bGDIInitialized = false;

	FORCEINLINE bool IsInScreen(const ScreenPoint& InPos) const;
	FORCEINLINE int GetIndexFromScreenPoint(const ScreenPoint& InPos) const
	{
		return ScreenSize.X * (TopLeftPoint.Y - InPos.Y) + (InPos.X - TopLeftPoint.X);
	}

	void SetPixel(const ScreenPoint& InPos, const Color32& InColor);

	HWND Wnd;
	HDC	ScreenDC, MemoryDC;
	HBITMAP DefaultBitmap, DIBitmap;

	Color32* ScreenBuffer = nullptr;
	float* DepthBuffer = nullptr;
	Color32 CurrentColor;

	ScreenPoint ScreenSize;
	ScreenPoint TopLeftPoint;
	ScreenPoint BottomRightPoint;
};
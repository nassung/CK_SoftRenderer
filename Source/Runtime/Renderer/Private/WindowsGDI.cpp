
#include "Precompiled.h"
#include "WindowsGDI.h"

void WindowsGDI::InitializeGDI(const ScreenPoint& InDisplaySize, const bool InbSRGBColorSpace)
{
	Wnd = ::GetActiveWindow();
	if (Wnd == NULL)
	{
		return;
	}

	if (bGDIInitialized)
	{
		DeleteObject(DefaultBitmap);
		DeleteObject(DIBitmap);
		ReleaseDC(Wnd, ScreenDC);
		ReleaseDC(Wnd, MemoryDC);
	}

	ScreenDC = GetDC(Wnd);
	if (ScreenDC == NULL)
	{
		return;
	}

	MemoryDC = CreateCompatibleDC(ScreenDC);
	if (MemoryDC == NULL)
	{
		return;
	}

	// Screen & Screen Coordinate Setting
	ScreenSize = InDisplaySize;
	bool isXEven = (ScreenSize.X % 2 == 0);
	bool isYEven = (ScreenSize.Y % 2 == 0);

	int halfXSize = Math::RoundToInt(ScreenSize.X * 0.5f);
	int halfYSize = Math::RoundToInt(ScreenSize.Y * 0.5f);

	TopLeftPoint = ScreenPoint(-halfXSize + 1, halfYSize - 1);
	BottomRightPoint = ScreenPoint(isXEven ? halfXSize : halfXSize - 1, isYEven ? -halfYSize : -halfYSize + 1);

	// Color & Bitmap Setting
	bSRGBColorSpace = InbSRGBColorSpace;

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = ScreenSize.X;
	bmi.bmiHeader.biHeight = -ScreenSize.Y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	DIBitmap = CreateDIBSection(MemoryDC, &bmi, DIB_RGB_COLORS, (void**)&ScreenBuffer, NULL, 0);
	if (DIBitmap == NULL)
	{
		return;
	}

	DefaultBitmap = (HBITMAP)SelectObject(MemoryDC, DIBitmap);
	if (DefaultBitmap == NULL)
	{
		return;
	}

	bGDIInitialized = true;
}

void WindowsGDI::ReleaseGDI()
{
	if (bGDIInitialized)
	{
		DeleteObject(DefaultBitmap);
		DeleteObject(DIBitmap);
		ReleaseDC(Wnd, ScreenDC);
		ReleaseDC(Wnd, MemoryDC);
	}

	if (DepthBuffer != nullptr)
	{
		delete[] DepthBuffer;
		DepthBuffer = nullptr;
	}

	bGDIInitialized = false;
}


void WindowsGDI::FillBuffer()
{
	if (!bGDIInitialized || (ScreenBuffer == NULL))
	{
		return;
	}

	Color32* dest = ScreenBuffer;
	UINT totalCount = ScreenSize.X * ScreenSize.Y;
	CopyBuffer(ScreenBuffer, &CurrentColor, totalCount);
	return;
}

Color32* WindowsGDI::CopyBuffer(Color32* InDst, Color32* InSrc, int InCount)
{
	if (InCount == 0)
	{
		return NULL;
	}

	if (InCount == 1)
	{
		memcpy(InDst, InSrc, sizeof(Color32));
	}
	else
	{
		int half = InCount / 2;
		CopyBuffer(InDst, InSrc, half);
		memcpy(InDst + half, InDst, half * sizeof(Color32));

		if (InCount % 2)
		{
			memcpy(InDst + (InCount - 1), InSrc, sizeof(Color32));
		}
	}

	return InDst;
}

void WindowsGDI::SetColor(const LinearColor& InColor)
{
	CurrentColor = InColor.ToColor32(bSRGBColorSpace);
}

LinearColor WindowsGDI::GetPixel(const ScreenPoint& InPos)
{
	if (!IsInScreen(InPos))
	{
		return LinearColor::Error;
	}

	Color32* dest = ScreenBuffer;
	Color32 bufferColor = *(dest + GetIndexFromScreenPoint(InPos));
	return LinearColor(bufferColor);
}

void WindowsGDI::SetPixel(const ScreenPoint& InPos)
{
	SetPixel(InPos, CurrentColor);
}

FORCEINLINE bool WindowsGDI::IsInScreen(const ScreenPoint& InPos) const
{
	int position = GetIndexFromScreenPoint(InPos);
	if (position < 0 || position >= (ScreenSize.X * ScreenSize.Y))
	{
		return false;
	}

	return true;
}

void WindowsGDI::SetPixel(const ScreenPoint& InPos, const Color32& InColor)
{
	if (!IsInScreen(InPos))
	{
		return;
	}

	Color32* dest = ScreenBuffer;
	*(dest + GetIndexFromScreenPoint(InPos)) = InColor;
	return;
}


Color32* WindowsGDI::GetScreenBuffer() const
{
	return ScreenBuffer;
}


void WindowsGDI::SwapBuffer()
{
	if (!bGDIInitialized)
	{
		return;
	}

	BitBlt(ScreenDC, 0, 0, ScreenSize.X, ScreenSize.Y, MemoryDC, 0, 0, SRCCOPY);
}


void WindowsGDI::SetPixelWithoutAlpha(const ScreenPoint& InPos, const LinearColor& InColor)
{
	SetPixel(InPos, InColor.ToColor32(bSRGBColorSpace));
}

void WindowsGDI::SetPixelWithAlpha(const ScreenPoint & InPos, const LinearColor & InColor)
{
	LinearColor bufferColor = GetPixel(InPos);
	SetPixel(InPos, (InColor * InColor.A + bufferColor * (1.f - InColor.A)).ToColor32());
}

void WindowsGDI::CreateDepthBuffer()
{
	DepthBuffer = new float[ScreenSize.X * ScreenSize.Y];
}

void WindowsGDI::ClearDepthBuffer()
{
	if (DepthBuffer != nullptr)
	{
		float* current = DepthBuffer;
		float defValue = INFINITY;
		unsigned long totalCount = ScreenSize.X * ScreenSize.Y;
		while (totalCount--)
		{
			*current++ = defValue;
		}
	}
}

float WindowsGDI::GetDepthBufferValue(const ScreenPoint& InPos) const
{
	if (DepthBuffer == nullptr)
	{
		return INFINITY;
	}

	if (!IsInScreen(InPos))
	{
		return INFINITY;
	}

	return *(DepthBuffer + GetIndexFromScreenPoint(InPos));
}

void WindowsGDI::SetDepthBufferValue(const ScreenPoint& InPos, float InDepthValue)
{
	if (DepthBuffer == nullptr)
	{
		return;
	}

	if (!IsInScreen(InPos))
	{
		return;
	}

	*(DepthBuffer + GetIndexFromScreenPoint(InPos)) = InDepthValue;
}
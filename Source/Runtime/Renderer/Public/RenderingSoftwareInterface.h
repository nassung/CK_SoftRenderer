
#pragma once

#include "DisplaySetting.h"
#include "LinearColor.h"

struct VertexData
{
	Vector2 Position;
	LinearColor Color = LinearColor::Error;
	Vector2 UV;
};

class RenderingSoftwareInterface
{
public:
	virtual void Init(const bool InbSRGBColorSpace = true) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsIntialized() const = 0;
	virtual bool IsSRGBSpace() const = 0;

	virtual void Clear(const LinearColor& InClearColor) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void DrawScreenPoint(const ScreenPoint& InScreenPos, const LinearColor& InColor) = 0;
	virtual void DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor, int InThickness = 1) = 0;
	virtual void DrawArrowLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor, int InThickness = 1) = 0;
	virtual void DrawVerticalLine(int InX, const LinearColor& InColor) = 0;
	virtual void DrawHorizontalLine(int InY, const LinearColor& InColor) = 0;
	virtual void DrawHorizontalLine(const ScreenPoint& InStartPos, const ScreenPoint& InEndPos, const LinearColor& InColor) = 0;

	virtual void SetVertexBuffer(VertexData* InVertexData) = 0;
	virtual void SetIndexBuffer(const int* InIndexData) = 0;
	virtual void DrawPrimitive(UINT InVertexSize, UINT InIndexSize) = 0;
};

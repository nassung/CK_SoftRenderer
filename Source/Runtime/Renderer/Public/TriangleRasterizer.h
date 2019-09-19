#pragma once

#include "Vector2.h"
#include "LinearColor.h"
#include "RenderingSoftwareInterface.h"

class TriangleRasterizer
{
public:
	TriangleRasterizer();
	~TriangleRasterizer();
	explicit TriangleRasterizer(VertexData InVertex0, VertexData InVertex1, VertexData InVertex2);

public:
	FORCEINLINE bool HasColor() const { return bHasColor; }
	FORCEINLINE bool HasUV() const { return bHasUV; }
	void RecalcBounds();
	void GenerateContourBuffer(const ScreenPoint& InStartPos, const ScreenPoint& InEndPos);
	void CheckAndSetContourValue(int currentX, int currentYIndex);
	int* GetContourBuffer() const { return ContourBuffer; }
	int GetContourBufferSize() const { return ContourBufferSize; }

	FORCEINLINE bool IsInside(const Vector2& In) const
	{
		Vector2 st = GetBaryCentricCoord(In);
		if (st.X < 0.f) return false;
		if (st.Y < 0.f) return false;
		if ((st.X + st.Y) > 1.f) return false;

		return true;	
	}

	FORCEINLINE LinearColor GetColor(const Vector2& InScreenPosition) const
	{
		if (!HasColor())
		{
			return LinearColor::Error;
		}

		Vector2 st = GetBaryCentricCoord(InScreenPosition);
		float oneMinusST = 1 - st.X - st.Y;
		return VertexBuffer[0].Color * oneMinusST + VertexBuffer[1].Color * st.X + VertexBuffer[2].Color * st.Y;
	}

	FORCEINLINE Vector2 GetBaryCentricCoord(const Vector2& InScreenPosition) const
	{
		Vector2 w = InScreenPosition - Vector2(VertexBuffer[0].Position.X, VertexBuffer[0].Position.Y);
		float dotUW = UVector.Dot(w);
		float dotVW = VVector.Dot(w);
		float s = (DotVV * dotUW - DotUV * dotVW) * InvDenom;
		float t = (DotUU * dotVW - DotUV * dotUW) * InvDenom;
		return Vector2(s, t);
	}

public:
	VertexData VertexBuffer[3];
	ScreenPoint BottomLeft;
	ScreenPoint TopRight;

private:
	Vector2 UVector;
	Vector2 VVector;
	float DotUU, DotUV, DotVV, InvDenom;

	int* ContourBuffer = nullptr;
	int ContourBufferSize = 0;

	bool bHasVertex = false;
	bool bHasColor = false;
	bool bHasUV = false;
};

#pragma once

#include "CoreDefinition.h"
#include "Vector2.h"

struct ScreenPoint
{
public:
	ScreenPoint() : X(0), Y(0) { }
	explicit ScreenPoint(int InX, int InY) : X(InX), Y(InY) { }
	explicit ScreenPoint(float InX, float InY) 
	{
		X = Math::CeilToInt(InX);
		Y = Math::FloorToInt(InY);
	}
	explicit ScreenPoint(const Vector2& InPos) : ScreenPoint(InPos.X, InPos.Y) {}

	FORCEINLINE Vector2 ToVector2() const
	{
		return Vector2((float)X - 0.5f, (float)Y + 0.5f);
	}
	FORCEINLINE ScreenPoint operator-(const ScreenPoint& InPoint) const;
	FORCEINLINE ScreenPoint operator+(const ScreenPoint& InPoint) const;

	int X;
	int Y;
};

FORCEINLINE ScreenPoint ScreenPoint::operator-(const ScreenPoint& InPoint) const
{
	return ScreenPoint(X - InPoint.X, Y - InPoint.Y);
}

FORCEINLINE ScreenPoint ScreenPoint::operator+(const ScreenPoint& InPoint) const
{
	return ScreenPoint(X + InPoint.X, Y + InPoint.Y);
}
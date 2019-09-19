
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"
#include "WindowsTimer.h"

void SoftRenderer::Initialize()
{
	// Init System Timer
	MilliSecFrequency = WindowsTimer::Init();
	if (MilliSecFrequency == 0)
		return;

	FrameCount = 0;
	ElapsedTime = 0;

	// Init RSI
	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);
	}
}

void SoftRenderer::Shutdown()
{
	if (RSI != nullptr && RSI->IsIntialized())
	{
		RSI->Shutdown();
		delete RSI;
		RSI = nullptr;
	}
}

void SoftRenderer::Update()
{
	if (RSI == nullptr) return;


	// Colors to use
	LinearColor vectorSpaceColor(LinearColor(0.125f, 0.5f, 1.0f, 0.8f));

	// Basis Vector forms Vector Space
	Vector2 xBasisVector(1.f, 0.f);
	Vector2 yBasisVector(0.f, 1.f);

	// Scalar to use
	float scalar = 100.f;

	// To Do : Define Transform Matrix 2x2 


	// Define Mesh Data
	VertexData v[4];
	v[0].Position =  Vector2::Zero;
	v[0].Color = vectorSpaceColor;

	v[1].Position = xBasisVector * scalar;
	v[1].Color = vectorSpaceColor;

	v[2].Position = yBasisVector * scalar;
	v[2].Color = vectorSpaceColor;

	v[3].Position = (xBasisVector + yBasisVector) * scalar;
	v[3].Color = vectorSpaceColor;

	int i[6];
	i[0] = 0;
	i[1] = 3;
	i[2] = 1;
	i[3] = 0;
	i[4] = 2;
	i[5] = 3;

	// Draw Call
	RSI->SetVertexBuffer(v);
	RSI->SetIndexBuffer(i);
	RSI->DrawPrimitive(4, 6);
}

void SoftRenderer::PreUpdate()
{
	StartFrameSec = WindowsTimer::CheckMilliSeconds();

	if (RSI == nullptr) return;

	// Colors to use
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.5f));

	// Render Start
	static int gridSize = 10;
	RSI->Clear(LinearColor::White);

	// Draw Grid Line
	ScreenPoint screenSize = DisplaySetting::Inst().GetSize();

	int startX = -Math::RoundToInt(screenSize.X * 0.5f);
	int endX = -startX;

	int startY = -Math::RoundToInt(screenSize.Y * 0.5f);
	int endY = -startY;

	ScreenPoint xAxisStart(startX, 0);
	ScreenPoint xAxisEnd(endX, 0);
	ScreenPoint yAxisStart(0, startY);
	ScreenPoint yAxisEnd(0, endY);

	for (int x = 0; x <= endX; x += gridSize)
	{
		RSI->DrawVerticalLine(x, gridColor);
		if (x > 0)
		{
			RSI->DrawVerticalLine(-x, gridColor);
		}
	}

	for (int y = 0; y <= endY; y += gridSize)
	{
		RSI->DrawHorizontalLine(y, gridColor);
		if (y > 0)
		{
			RSI->DrawHorizontalLine(-y, gridColor);
		}
	}

	// Draw World Axis
	RSI->DrawHorizontalLine(0, LinearColor::Red);
	RSI->DrawVerticalLine(0, LinearColor::Green);
}

void SoftRenderer::PostUpdate()
{
	if (RSI != nullptr)
	{
		// Render Finish
		RSI->EndFrame();
	}

	// Check Timer and Increase Frame Stat
	FrameCount++;
	FrameMilliSec = double(WindowsTimer::CheckMilliSeconds() - StartFrameSec) / MilliSecFrequency;
	FrameSec = (float)(FrameMilliSec / 1000);
	ElapsedTime += FrameSec;

	FrameFPS = FrameSec == 0 ? 0 : 1 / FrameSec;
}



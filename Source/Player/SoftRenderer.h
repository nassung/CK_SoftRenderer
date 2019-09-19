#pragma once

#include "WindowsRSI.h"

class SoftRenderer
{
public:
	enum class RenderMode
	{
		TWO,
		THREE_PERSP
	};

	SoftRenderer(SoftRenderer const&) = delete;
	void operator=(SoftRenderer const&) = delete;
	static SoftRenderer& Inst()
	{
		static SoftRenderer instance;
		return instance;
	}

public:
	void SetRenderMode(RenderMode InRenderMode) { CurrentRenderMode = InRenderMode; }
	RenderMode GetRenderMode() const { return CurrentRenderMode; }
	void Initialize();
	void Shutdown();
	void Update();

	void PreUpdate();
	void PostUpdate();

	float GetFrameFPS() const { return FrameFPS; }
	float GetAverageFPS() const { return FrameCount == 0 ? 0.0f : FrameCount / ElapsedTime; }
	float GetElapsedTime() const { return ElapsedTime; }
	int GetFrameCount() const { return FrameCount; }

private:
	SoftRenderer() { }
	~SoftRenderer() { Shutdown(); }

	double MilliSecFrequency = 0;
	double FrameMilliSec = 0;
	__int64 StartFrameSec = 0;
	float FrameSec = 0;
	float FrameFPS = 0;
	float ElapsedTime = 0;
	int FrameCount = 0;

	RenderMode CurrentRenderMode = RenderMode::TWO;

	RenderingSoftwareInterface* RSI = nullptr;
};

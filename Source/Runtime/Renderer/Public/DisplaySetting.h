
#pragma once

#include "ScreenPoint.h"

class DisplaySetting
{
public:
	DisplaySetting(DisplaySetting const&) = delete;
	void operator=(DisplaySetting const&) = delete;
	static DisplaySetting& Inst()
	{
		static DisplaySetting instance;
		return instance;
	}

public:
	void SetSize(ScreenPoint InSize)
	{
		Size = InSize;
	}

	ScreenPoint GetSize() const
	{
		return Size;
	}

	int GetPixelCount() const
	{
		return Size.X * Size.Y;
	}

private:
	DisplaySetting() { };

	ScreenPoint Size;
};


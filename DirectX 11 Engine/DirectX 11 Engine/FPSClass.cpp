///////////////////////////////////////////////////////////////////////////////
// Filename: fpsclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "fpsclass.h"


FpsClass::FpsClass()
{
}


FpsClass::FpsClass(const FpsClass& other)
{
}


FpsClass::~FpsClass()
{
}


void FpsClass::Initialize()
{
	this->fps = 0;
	this->count = 0;
	this->startTime = timeGetTime();
	return;
}


void FpsClass::Frame()
{
	this->count++;

	if (timeGetTime() >= (this->startTime + 1000))
	{
		this->fps = this->count;
		this->count = 0;

		this->startTime = timeGetTime();
	}
}


int FpsClass::GetFps()
{
	return this->fps;
}
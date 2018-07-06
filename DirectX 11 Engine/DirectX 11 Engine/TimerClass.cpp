///////////////////////////////////////////////////////////////////////////////
// Filename: timerclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "timerclass.h"


TimerClass::TimerClass()
{
}


TimerClass::TimerClass(const TimerClass& other)
{
}


TimerClass::~TimerClass()
{
}


bool TimerClass::Initialize()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&this->frequency);
	if (this->frequency == 0)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	this->ticksPerMs = (float)(this->frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->startTime);

	return true;
}


void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDifference;


	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - this->startTime);

	this->frameTime = timeDifference / this->ticksPerMs;

	this->startTime = currentTime;

	return;
}


float TimerClass::GetTime()
{
	return this->frameTime;
}
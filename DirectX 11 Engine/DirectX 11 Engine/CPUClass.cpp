///////////////////////////////////////////////////////////////////////////////
// Filename: cpuclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "cpuclass.h"


CpuClass::CpuClass()
{
}


CpuClass::CpuClass(const CpuClass& other)
{
}


CpuClass::~CpuClass()
{
}


void CpuClass::Initialize()
{
	PDH_STATUS status;


	// Initialize the flag indicating whether this object can read the system cpu usage or not.
	this->canReadCpu = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &this->queryHandle);
	if (status != ERROR_SUCCESS)
	{
		this->canReadCpu = false;
	}

	// Set query object to poll all cpus in the system.
	status = PdhAddCounter(this->queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &this->counterHandle);
	if (status != ERROR_SUCCESS)
	{
		this->canReadCpu = false;
	}

	this->lastSampleTime = GetTickCount();

	this->cpuUsage = 0;

	return;
}


void CpuClass::Shutdown()
{
	if (this->canReadCpu)
	{
		PdhCloseQuery(this->queryHandle);
	}

	return;
}


void CpuClass::Frame()
{
	PDH_FMT_COUNTERVALUE value;

	if (this->canReadCpu)
	{
		if ((this->lastSampleTime + 1000) < GetTickCount())
		{
			this->lastSampleTime = GetTickCount();

			PdhCollectQueryData(this->queryHandle);

			PdhGetFormattedCounterValue(this->counterHandle, PDH_FMT_LONG, NULL, &value);

			this->cpuUsage = value.longValue;
		}
	}

	return;
}


int CpuClass::GetCpuPercentage()
{
	int usage;

	if (this->canReadCpu)
	{
		usage = (int)this->cpuUsage;
	}
	else
	{
		usage = 0;
	}

	return usage;
}
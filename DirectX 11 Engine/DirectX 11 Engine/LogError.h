#pragma once
#include <Windows.h>
#include <string>
#include <comdef.h>

static void LogError(std::string error)
{
	MessageBoxA(NULL, error.c_str(), error.c_str(), MB_ICONERROR);
}

static std::string HRToString(HRESULT hr)
{
	return "";
	/*_com_error err(hr);
	LPCTSTR errMsg = err.ErrorMessage();
	std::wstring wstr = errMsg;
	return std::string(wstr.begin(),wstr.end());*/
}
#pragma once
#include "Graphics.h"
#include "IncludeUtilities.h"
#include "MouseClass.h"
#include "KeyboardClass.h"

class Engine
{
public:
	bool Initialize(HINSTANCE hInstance, int width, int height);
	bool ProcessMessages();
	bool Update();
	void RenderFrame();
	void Release();
	LRESULT CALLBACK HandleMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	RAWINPUTDEVICE rid;
	MousePoint prevMousePos;
	float dt = 0;
	MouseClass mouse;
	KeyboardClass kbd;
	bool InitializeWindow();
	bool InitializeGraphics();
	int m_height = 0;
	int m_width = 0;
	HWND m_hwnd = NULL;
	HINSTANCE m_hInstance = NULL;
	Graphics m_gfx;
	TimerClass m_Timer;

	RECT wr;
};
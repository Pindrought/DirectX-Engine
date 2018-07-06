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
	int height = 0;
	int width = 0;
	HWND hwnd = NULL;
	HINSTANCE hInstance = NULL;
	Graphics gfx;
	TimerClass Timer;

	RECT wr;
};
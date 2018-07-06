#include "Engine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Engine engine;
	bool success = engine.Initialize(hInstance, 800, 600);
	if (success == true)
	{
		while (engine.ProcessMessages())
		{
			if (!engine.Update())
				break;
			engine.RenderFrame();
		}
	}
	engine.Release();


	return 0;
}
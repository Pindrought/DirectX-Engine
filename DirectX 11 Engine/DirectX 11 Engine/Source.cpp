#include "Engine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Engine engine;
	bool success = engine.Initialize(hInstance, 800, 600);
	Timer timer;
	double desiredFps = 144;

	if (success == true)
	{
		timer.Start();
		while (engine.ProcessMessages())
		{
			if (!engine.Update())
				break;

			double timeDifference = (1000.0 / desiredFps) - timer.GetMilisecondsElapsed();
			if (timeDifference < 1.0) 
			{
				engine.RenderFrame();
				timer.Restart();
			}
			else
			{
				Sleep(1);
			}
		}
	}
	engine.Release();


	return 0;
}
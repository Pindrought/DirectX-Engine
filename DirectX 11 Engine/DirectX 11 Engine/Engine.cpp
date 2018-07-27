#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, int width, int height)
{
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;


	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;   
	rid.hwndTarget = 0;

	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE) 
	{
		return false;
		//registration failed. Call GetLastError for the cause of the error
	}

	this->Timer.Initialize();

	if (InitializeWindow() == false)
		return false;

	if (InitializeGraphics() == false)
		return false;

	return true;
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// Check if the window is being destroyed.

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	// All other messages
	default:
	{
		// retrieve ptr to window class
		Engine* const pEngine = reinterpret_cast<Engine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		// forward message to window class handler
		return pEngine->HandleMsg(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (uMsg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Engine* const pEngine = reinterpret_cast<Engine*>(pCreate->lpCreateParams);
		// sanity check
		assert(pEngine != nullptr);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pEngine));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		// forward message to window class handler
		return pEngine->HandleMsg(hwnd, uMsg, wParam, lParam);
	}
	// if we get a message before the Wthis->NCCREATE message, handle with default handler
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK Engine::HandleMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//Keyboard Events
		case WM_KEYDOWN:
		{
			if (kbd.IsKeysAutoRepeat() || !(lParam & 0x40000000)) //If AutoRepeat is on for keys, or if the key is not a repeated key
			{
				kbd.OnKeyPressed((unsigned char)wParam);
			}
			return 0;
		}

		case WM_KEYUP:
		{
			kbd.OnKeyReleased((unsigned char)wParam);
			return 0;
		}

		case WM_CHAR:
		{
			if (kbd.IsCharsAutoRepeat() || !(lParam & 0x40000000)) //If AutoRepeat is on for chars, or if the key is not a repeated key
			{
				kbd.OnChar((unsigned char)wParam);
			}
			return 0;
		}

		//MOUSE EVENTS
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if (x >= 0 && x < this->width && y >= 0 && y < this->height)
			{
				mouse.OnMouseMove(x, y);
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnLeftPressed(x, y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnRightPressed(x, y);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnMiddlePressed(x, y);
			break;
		}
		case WM_LBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnLeftReleased(x, y);
			break;
		}
		case WM_RBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnRightReleased(x, y);
			break;
		}
		case WM_MBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse.OnMiddleReleased(x, y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				mouse.OnWheelUp(x, y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				mouse.OnWheelDown(x, y);
			}
			break;
		}

		case WM_INPUT:
		{
			UINT dwSize;

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,  sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			if (lpb == NULL)
			{
				return 0;
			}

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			{
				OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
			}

			RAWINPUT* raw = (RAWINPUT*)lpb;
			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0)
				{
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					//SetCursorPos((wr.right - wr.left) / 2 + wr.left, (wr.bottom - wr.top) / 2 + wr.top);
				}
				//HRESULT hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
				/*raw->data.mouse.usFlags,
				raw->data.mouse.ulButtons,
				raw->data.mouse.usButtonFlags,
				raw->data.mouse.usButtonData,
				raw->data.mouse.ulRawButtons,
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY,
				raw->data.mouse.ulExtraInformation);*/

				//if (FAILED(hResult))
				//{
				//	// TODO: write error handler
				//}
				//OutputDebugString(szTempOutput);
			}

			delete[] lpb;
			return 0;
		}

		// All other messages pass to the Default Window Proc
		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

bool Engine::InitializeWindow()
{
	if (this->hInstance == NULL)
	{
		LogError("Engine::InitializeWindow Failed - hInstance is NULL");
		return false;
	}

	WNDCLASSEX wc; //Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	wc.lpfnWndProc = HandleMessageSetup; //Pointer to Window Proc function for handling messages from this window
	wc.cbClsExtra = 0; //# of extra bytes to allocate following the window-class structure. We are not currently using this.
	wc.cbWndExtra = 0; //# of extra bytes to allocate following the window instance. We are not currently using this.
	wc.hInstance = this->hInstance; //Handle to the instance that contains the Window Procedure
	wc.hIcon = NULL;   //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
	wc.hIconSm = NULL; //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
	wc.hbrBackground = NULL; //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
	wc.lpszMenuName = NULL; //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
	wc.lpszClassName = L"Winclass"; //Pointer to null terminated string of our class name for this window.
	wc.cbSize = sizeof(WNDCLASSEX); //Need to fill in the size of our struct for cbSize

	RegisterClassEx(&wc); // Register the class so that it is usable.

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Place the window in the middle of the screen.
	int posX = (screenWidth - this->width) / 2; //Left position of Window
	int posY = (screenHeight - this->height) / 2; //Top position of Window

											// Set up rectangle for our window dimensions/location
	
	wr.left = posX;
	wr.right = this->width + wr.left;
	wr.top = posY;
	wr.bottom = this->height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// Create the window with the screen settings and get the handle to it. For more information on the function itself, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632680(v=vs.85).aspx
	HWND hwnd = CreateWindowEx(0, //Extended Windows style - we are using the default. For other options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ff700543(v=vs.85).aspx
		wc.lpszClassName, //Window class name
		L"WinTitle", //Window Title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //Windows style - See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632600(v=vs.85).aspx
		wr.left, //Window X Position
		wr.top, //Window Y Position
		wr.right - wr.left, //Window Width
		wr.bottom - wr.top, //Window Height
		NULL, //Handle to parent of this window. Since this is the first window, it has no parent window.
		NULL, //Handle to menu or child window identifier. Can be set to NULL and use menu in WindowClassEx if a menu is desired to be used.
		this->hInstance, //Handle to the instance of module to be used with this window
		this); //Param to create window

	if (hwnd == NULL) //If window failed to create
	{
		LogError("CreateWindowEx Failed! Error #: " + std::to_string(GetLastError()));
		return false;
	}

	this->hwnd = hwnd;

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return true;
}

bool Engine::InitializeGraphics()
{
	if (this->gfx.Initialize(this->hwnd,this->width,this->height) == false)
		return false;
	return true;
}

bool Engine::ProcessMessages()
{
	// Handle the windows messages.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.

	if (PeekMessage(&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
					this->hwnd, //Handle to window we are checking messages for
					0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
					0,    //Maximum Filter Msg Value
					PM_REMOVE))//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
	{
		TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
		DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
	}

	// If windows signals to end the application then exit out.
	if (msg.message == WM_QUIT)
	{
		return false;
	}
	return true;
}

bool Engine::Update()
{
	this->Timer.Frame();
	dt = this->Timer.GetTime();

	//If input to be processed by keyboard class
	while (kbd.CharBufferIsEmpty() == false) //If characters to be processed
	{
		char ch = kbd.ReadChar(); //Get character for processing
	}
	while (kbd.KeyBufferIsEmpty() == false) //If key presses/releases need to be processed
	{
		KeyboardEvent kbe = kbd.ReadKey(); //Get keyboard event for processing
		if (kbe.GetKeyCode() == VK_ESCAPE)
		{
			return false;
		}
	}

	static float cubeRotThing = -0.001f;

	while (mouse.EventBufferIsEmpty() == false) //if mouse events need to be processed
	{
		MouseEvent me = mouse.ReadEvent(); //get mouse event for processing
		MouseEvent::EventType et = me.GetType();
		//if (et == MouseEvent::EventType::LPress)
		//{
		//	prevMousePos = mouse.GetPos(); //Get initial mouse pos to compare when turning camera
		//}
		if (et == MouseEvent::EventType::RAW_MOVE)
		{
			this->gfx.camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f,0);
			//prevMousePos = me.GetPos();
		}
		if (et == MouseEvent::EventType::RPress)
		{
			float xValue = ((float)me.GetPosX() / (float)this->width) * 2 - 1;
			float yValue = ((float)me.GetPosY() / (float)this->height) * 2 - 1;
			float zValue = 1;
			const XMMATRIX cam = this->gfx.camera.GetProjectionMatrix();
			xValue /= cam.r[0].m128_f32[0];
			yValue /= -cam.r[1].m128_f32[1];
			XMVECTOR pickRayInViewSpaceDir = XMVectorSet(xValue, yValue, zValue, 0.0f);
			pickRayInViewSpaceDir = XMVector3Normalize(pickRayInViewSpaceDir);
			XMVECTOR pickRayInViewSpacePos = XMVectorSet(0, 0, 0, 0);
			XMMATRIX viewInverse = XMMatrixInverse(nullptr, this->gfx.camera.GetViewMatrix());
			XMVECTOR pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, viewInverse);
			XMVECTOR pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, viewInverse);

			XMVECTOR vecToModel = this->gfx.cube.GetPos() - this->gfx.camera.GetPosition();

			XMVECTOR vecLength = XMVector3Length(vecToModel);

			vecToModel = XMVector3Normalize(vecToModel);

			XMVECTOR dotResult = XMVector3Dot(vecToModel, pickRayInWorldSpaceDir);

			XMVECTOR pickDestination = pickRayInWorldSpacePos + pickRayInWorldSpaceDir * dotResult * vecLength;

			XMVECTOR vecFromPickDestToModel = this->gfx.cube.GetPos() - pickDestination;

			if (XMVector3Length(vecFromPickDestToModel).m128_f32[0] < 1.4f)
			{
				this->gfx.drawText = "Intersection";
				cubeRotThing = -cubeRotThing;
			}
			else
			{
				this->gfx.drawText = "No intersection";
			}
		}
		else
		{
			this->gfx.drawText = "N/A";
		}
	}

	
	

	if (kbd.KeyIsPressed('W'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * dt * 0.02f);
	}
	if (kbd.KeyIsPressed('S'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * dt * 0.02f);
	}
	if (kbd.KeyIsPressed('A'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * dt * 0.02f);
	}
	if (kbd.KeyIsPressed('D'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * dt * 0.02f);
	}
	if (kbd.KeyIsPressed(VK_SPACE))
	{
		this->gfx.camera.AdjustPosition({ 0.0f, dt * 0.02f, 0.0f });
	}
	if (kbd.KeyIsPressed('Z'))
	{
		this->gfx.camera.AdjustPosition({ 0.0f, -dt * 0.02f, 0.0f });
	}
	if (kbd.KeyIsPressed(VK_RIGHT))
	{
		this->gfx.camera.AdjustRotation(0, 0.002f*dt, 0);
	}
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		this->gfx.camera.AdjustRotation(0, -0.002f*dt, 0);
	}
	if (kbd.KeyIsPressed(VK_UP))
	{
		this->gfx.camera.AdjustRotation(-0.002f*dt, 0, 0);
	}
	if (kbd.KeyIsPressed(VK_DOWN))
	{
		this->gfx.camera.AdjustRotation(0.002f*dt, 0, 0);
	}

	this->gfx.cube.AdjustRotation(0, cubeRotThing*dt, 0);


	return true;
}

void Engine::RenderFrame()
{
	

	//Code to rotate model

	this->gfx.skybox.SetPos(this->gfx.camera.GetPosition());

	this->gfx.RenderFrame(dt);
}

void Engine::Release()
{
	this->gfx.Release();
	if (this->hwnd != NULL)
	{
		DestroyWindow(this->hwnd);
		UnregisterClass(L"WinClass", this->hInstance);
		this->hwnd = NULL;
		this->hInstance = NULL;
	}
}
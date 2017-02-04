#define INTERNAL 1
#define WIN32 1


#include <GL\glew.h>
#include <Windows.h>
#include <mmsystem.h>

#include <stdio.h>
#include <iostream>
#include <stdint.h>

#include "Input.h"
#include "Game.h"
#include "Junk.h"

#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

// NOTE(Jonny): This is here because, despite Visual Studio includisng EVERY DLL BY DEFAULT
//				they actually don't. We need this for the timebeginPeriod function to make
//				sleep more precise
#pragma comment(lib, "Winmm.lib")

#define CLASS_NAME "Game"


struct GameMemory
{
	uint64_t storageSize;
	void *storage;

	bool isInitialized;
};

struct WindowInfo
{
	int width, height;
	bool focussed = false;
};

static int64_t g_performanceCountFrequency;
static WindowInfo windowInfo;


static LARGE_INTEGER win32GetWallClock(void)
{
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	return result;
}

inline float win32GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
	float result = ((float)(end.QuadPart - start.QuadPart) /
	                (float)g_performanceCountFrequency);
	return result;
}

static LRESULT CALLBACK win32MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch(message)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
		} break;

		default:
		{
			result = DefWindowProc(window, message, wParam, lParam);
		}
	}

	return result;
}


static void win32ProcessPendingMessages(Input &input, HWND window)
{	
	if (windowInfo.focussed && GetCapture() != window)
	{
		SetCapture(window);
	}

	POINT mousePoint;
	if (GetCursorPos(&mousePoint))
	{
		ScreenToClient(window, &mousePoint);
		input.MouseMoveDistance = -glm::vec2(windowInfo.width / 2 - mousePoint.x,
		                                      windowInfo.height / 2 - mousePoint.y);

		// NOTE(Kyle): Moved this here and fixed a problem. Not 100% sure why though. 
		if (windowInfo.focussed)
		{
			SetCursorPos(windowInfo.width / 2, windowInfo.height / 2);
		}
	}

	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{ 
		switch(message.message)
		{
			case WM_QUIT:
			{
				//exit(1);
			} break;

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				uint32_t vkCode = (uint32_t)message.wParam;
				switch (vkCode)
				{
					// NOTE(Jonny): YES I INDENTED THEM, IT ANNOYED ME. SO WHAT??
					case'Q': input.KeyDown[Q] = true; input.Key[Q] = true; break;
					case'W': input.KeyDown[W] = true; input.Key[W] = true; break;
					case'E': input.KeyDown[E] = true; input.Key[E] = true; break;
					case'R': input.KeyDown[R] = true; input.Key[R] = true; break;
					case'T': input.KeyDown[T] = true; input.Key[T] = true; break;
					case'Y': input.KeyDown[Y] = true; input.Key[Y] = true; break;
					case'U': input.KeyDown[U] = true; input.Key[U] = true; break;
					case'I': input.KeyDown[I] = true; input.Key[I] = true; break;
					case'O': input.KeyDown[O] = true; input.Key[O] = true; break;
					case'P': input.KeyDown[P] = true; input.Key[P] = true; break;
					case'A': input.KeyDown[A] = true; input.Key[A] = true; break;
					case'S': input.KeyDown[S] = true; input.Key[S] = true; break;
					case'D': input.KeyDown[D] = true; input.Key[D] = true; break;
					case'F': input.KeyDown[F] = true; input.Key[F] = true; break;
					case'G': input.KeyDown[G] = true; input.Key[G] = true; break;
					case'H': input.KeyDown[H] = true; input.Key[H] = true; break;
					case'J': input.KeyDown[J] = true; input.Key[J] = true; break;
					case'K': input.KeyDown[K] = true; input.Key[K] = true; break;
					case'L': input.KeyDown[L] = true; input.Key[L] = true; break;
					case'Z': input.KeyDown[Z] = true; input.Key[Z] = true; break;
					case'X': input.KeyDown[X] = true; input.Key[X] = true; break;
					case'C': input.KeyDown[C] = true; input.Key[C] = true; break;
					case'V': input.KeyDown[V] = true; input.Key[V] = true; break;
					case'B': input.KeyDown[B] = true; input.Key[B] = true; break;
					case'N': input.KeyDown[N] = true; input.Key[N] = true; break;
					case'M': input.KeyDown[M] = true; input.Key[M] = true; break;

					case VK_UP: input.KeyDown[UP] = true; input.Key[UP] = true; break;
					case VK_DOWN: input.KeyDown[DOWN] = true; input.Key[DOWN] = true; break;
					case VK_RIGHT: input.KeyDown[RIGHT] = true; input.Key[RIGHT] = true; break;
					case VK_LEFT: input.KeyDown[LEFT] = true; input.Key[LEFT] = true; break;

					case VK_SPACE: input.KeyDown[SPACE] = true; input.Key[SPACE] = true; break;
					case VK_RETURN: input.KeyDown[RETURN] = true; input.Key[RETURN] = true; break;
					case VK_ESCAPE: input.KeyDown[ESCAPE] = true; input.Key[ESCAPE] = true; break;
					case VK_DELETE: input.KeyDown[DEL] = true; input.Key[DEL] = true; break;
					case VK_BACK: input.KeyDown[BACKSPACE] = true; input.Key[BACKSPACE] = true; break;
				}
			} break;
			case WM_LBUTTONDOWN:
			{
				input.MouseButtonDown[MOUSE_LEFT] = true;
				input.MouseButton[MOUSE_LEFT] = true;
			} break;

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				uint32_t vkCode = (uint32_t)message.wParam;
				switch (vkCode)
				{
					// NOTE(Jonny): See comment above... :P
					case'Q': input.KeyUp[Q] = true; input.Key[Q] = false; break;
					case'W': input.KeyUp[W] = true; input.Key[W] = false; break;
					case'E': input.KeyUp[E] = true; input.Key[E] = false; break;
					case'R': input.KeyUp[R] = true; input.Key[R] = false; break;
					case'T': input.KeyUp[T] = true; input.Key[T] = false; break;
					case'Y': input.KeyUp[Y] = true; input.Key[Y] = false; break;
					case'U': input.KeyUp[U] = true; input.Key[U] = false; break;
					case'I': input.KeyUp[I] = true; input.Key[I] = false; break;
					case'O': input.KeyUp[O] = true; input.Key[O] = false; break;
					case'P': input.KeyUp[P] = true; input.Key[P] = false; break;
					case'A': input.KeyUp[A] = true; input.Key[A] = false; break;
					case'S': input.KeyUp[S] = true; input.Key[S] = false; break;
					case'D': input.KeyUp[D] = true; input.Key[D] = false; break;
					case'F': input.KeyUp[F] = true; input.Key[F] = false; break;
					case'G': input.KeyUp[G] = true; input.Key[G] = false; break;
					case'H': input.KeyUp[H] = true; input.Key[H] = false; break;
					case'J': input.KeyUp[J] = true; input.Key[J] = false; break;
					case'K': input.KeyUp[K] = true; input.Key[K] = false; break;
					case'L': input.KeyUp[L] = true; input.Key[L] = false; break;
					case'Z': input.KeyUp[Z] = true; input.Key[Z] = false; break;
					case'X': input.KeyUp[X] = true; input.Key[X] = false; break;
					case'C': input.KeyUp[C] = true; input.Key[C] = false; break;
					case'V': input.KeyUp[V] = true; input.Key[V] = false; break;
					case'B': input.KeyUp[B] = true; input.Key[B] = false; break;
					case'N': input.KeyUp[N] = true; input.Key[N] = false; break;
					case'M': input.KeyUp[M] = true; input.Key[M] = false; break;

					case VK_UP: input.KeyUp[UP] = true; input.Key[UP] = false; break;
					case VK_DOWN: input.KeyUp[DOWN] = true; input.Key[DOWN] = false; break;
					case VK_RIGHT: input.KeyUp[RIGHT] = true; input.Key[RIGHT] = false; break;
					case VK_LEFT: input.KeyUp[LEFT] = true; input.Key[LEFT] = false; break;
					
					case VK_SPACE: input.KeyUp[SPACE] = true; input.Key[SPACE] = false; break;
					case VK_RETURN: input.KeyUp[RETURN] = true; input.Key[RETURN] = false; break;
					case VK_ESCAPE: input.KeyUp[ESCAPE] = true; input.Key[ESCAPE] = false; break;
					case VK_DELETE: input.KeyUp[DEL] = true; input.Key[DEL] = false; break;
					case VK_BACK: input.KeyUp[BACKSPACE] = true; input.Key[BACKSPACE] = false; break;
				}

			} break;
			case WM_LBUTTONUP:
			{
				input.MouseButtonUp[MOUSE_LEFT] = true;
				input.MouseButton[MOUSE_LEFT] = false;
			}break;

			default:
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			} break;
		}
	}
}

// NOTE(Jonny): 	This code was nick'd from Raymon Chen's blog. Despite some of the
//					things I've heard about him, especially towards anyone doing
//					*anything* wrong in the Windows API, this code is pretty good
//					and works! No clue what it does though...
static HWND Win32CreateFullscreenWindow(HINSTANCE instance)
{
	HWND result = NULL;

	HMONITOR monitor = MonitorFromWindow(result, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo = { sizeof(monitorInfo) };

	if (GetMonitorInfo(monitor, &monitorInfo))
	{
		windowInfo.width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
		windowInfo.height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
		result = CreateWindow(TEXT("static"),
							  TEXT(""),
							  WS_POPUP | WS_VISIBLE,
							  monitorInfo.rcMonitor.left,
							  monitorInfo.rcMonitor.top,
							  windowInfo.width,
							  windowInfo.height,
							  NULL,
							  NULL,
							  instance,
							  0);
		windowInfo.focussed = true;
	}
	return result;
}

#define FULLSCREEN 0



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR whatever, int whatever2)
{
	LARGE_INTEGER performanceCountFrequencyResult;
	QueryPerformanceFrequency(&performanceCountFrequencyResult);
	g_performanceCountFrequency = performanceCountFrequencyResult.QuadPart;


	WNDCLASS windowClass;
	SecureZeroMemory(&windowClass, sizeof(windowClass));

	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = &win32MainWindowCallback;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = TEXT(CLASS_NAME);

	DEVMODE devMode;
	EnumDisplaySettings(NULL, 1, &devMode);

	uint32_t monitorRefreshRate = devMode.dmDisplayFrequency;
	uint32_t gameUpdateHz = monitorRefreshRate;
	float targetSecondsPerFrame = 1.0f / (float)gameUpdateHz;

	UINT desiredSchedulerMS = 1;
	timeBeginPeriod(desiredSchedulerMS);


	if (RegisterClass(&windowClass))
	{
		HWND window = NULL;
		#if 0
		window = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
								windowClass.lpszClassName,
								TEXT("Pizza Man Engine"),
								WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_VISIBLE | WS_POPUP,
								100, 100,
								800, 600,
								NULL,
								NULL,
								hInstance,
								NULL);
		#endif
		window = Win32CreateFullscreenWindow(hInstance);

		if (window != NULL)
		{
			HDC deviceContext = GetDC(window);

			PIXELFORMATDESCRIPTOR pixelFormatDescriptor;

			SecureZeroMemory(&pixelFormatDescriptor, sizeof(pixelFormatDescriptor));

			pixelFormatDescriptor.nSize			= sizeof(pixelFormatDescriptor);
			pixelFormatDescriptor.nVersion		= 1;
			pixelFormatDescriptor.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
			pixelFormatDescriptor.iPixelType	= PFD_TYPE_RGBA;
			pixelFormatDescriptor.cColorBits	= 24;
			pixelFormatDescriptor.cRedBits		= 8;
			pixelFormatDescriptor.cGreenBits	= 8;
			pixelFormatDescriptor.cBlueBits		= 8;
			pixelFormatDescriptor.cDepthBits	= 32;

			int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDescriptor);
			SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor);

			HGLRC renderContext = NULL;
			renderContext = wglCreateContext(deviceContext);

			if (renderContext != NULL)
			{
				wglMakeCurrent(deviceContext, renderContext);

				glewExperimental = GL_TRUE;
				GLenum err = glewInit();

				if (err == GLEW_OK)
				{
					GameMemory gameMemory = {};
					gameMemory.storageSize = Megabytes(64);
					gameMemory.storage = VirtualAlloc(NULL, (size_t)gameMemory.storageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

					if (gameMemory.storage)
					{
						LARGE_INTEGER lastCounter = win32GetWallClock();
						float msPerFrame = 0.0f;

						ShowCursor(false);
						Input input = {};	
						bool running = true;
						while (running)
						{
							input.ResetInput();
							win32ProcessPendingMessages(input, window);

							// NOTE(Kyle): This acts to inform our app if our window has focus or not. 
							HWND activeWindow = GetActiveWindow();
							if (activeWindow == NULL)
							{
								ReleaseCapture();
								windowInfo.focussed = false;
							}
							else
								windowInfo.focussed = true;

							// NOTE(Kyle): Make loop only run while the window is active. 
							// TODO(Jonny): Remember to uncomment this before release build 
							//if (windowInfo.focussed)
							{
								Assert(sizeof(GameState) < gameMemory.storageSize);
								GameState *gameState = (GameState*)gameMemory.storage;

								if (gameMemory.isInitialized == false)
								{
									gameMemory.isInitialized = true;
									initGame(gameState);
								}
								else
								{
									update(gameState, input, (msPerFrame/1000.0f));
									draw(gameState);
									SwapBuffers(deviceContext);


									// NOTE(Jonny): Windows API Frame-Rate Stuff
									LARGE_INTEGER workCounter = win32GetWallClock();
									float secondsElapsedForFrame = win32GetSecondsElapsed(lastCounter,
									                                                      workCounter);

									if (secondsElapsedForFrame < targetSecondsPerFrame)
									{
										DWORD sleepMS = (DWORD)(1000.0f * (targetSecondsPerFrame -
										                        		   secondsElapsedForFrame));
										if (sleepMS > 0)
										{
											Sleep(sleepMS);
										}
										else
										{
											// NOTE(Jonny): Missed Sleep
										}

										while (secondsElapsedForFrame < targetSecondsPerFrame)
										{
											secondsElapsedForFrame
												= win32GetSecondsElapsed(lastCounter,
												                         win32GetWallClock());
										}
									}
									LARGE_INTEGER endCounter = win32GetWallClock();
									msPerFrame = (1000.0f * win32GetSecondsElapsed(lastCounter,
									              								   endCounter));
									lastCounter = endCounter;
								}
							}
						}
					}
					else
					{
						printf("Failed to Init Game Memory");
					}
				}
				else
				{
					printf("GLEW failed to initialize");
				}
			}
			else
			{
				printf("Failed to create Render Context");
			}
		}
		else
		{
			printf("Window Failed to be created");
		}
	}
	else
	{
		printf("Class Failed to register");
	}

	ShowCursor(true);

	return 0;
}
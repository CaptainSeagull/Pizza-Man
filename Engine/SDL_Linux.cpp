#define NULL 0
#define GLM_FORCE_RADIANS

// For mmap
#include <sys/mman.h>

// SDL and GLEW
#include "Dependencies/include/GL/glew.h"
#include <SDL.h>

// GLM
#include "Dependencies/include/glm/glm.hpp"
#include "Dependencies/include/glm/gtc/matrix_transform.hpp"
#include "Dependencies/include/glm/gtc/type_ptr.hpp"

// Assimp
#include "Dependencies/include/assimp/Importer.hpp"
#include "Dependencies/include/assimp/scene.h"
#include "Dependencies/include/assimp/postprocess.h"
#include <vector>

// Standard Library
#include <iostream>

struct GameMemory
{
	uint64_t storageSize;
	void *storage;

	bool isInitialized;
};

#include "Game.h"
#include "stb_image.c"
#include <fstream>
#include "OpenGLHelper.h"


#include "Game.cpp"

// Libraries



// Source Files


#if INTERNAL
	#define Assert(x) if (!(x)) { *(int*)NULL=2; }
#else
	#define Assert(x)
#endif

#define Bytes(x)		((x) * (8LL))
#define Kilobytes(x)	((x) * (1024LL))
#define Megabytes(x)	((Kilobytes(x)) * (1024LL))


static void
SDLGetKeyboardInput(SDL_Event *event, Controls *controls)
{
	// switch(event->type)
	// {
	// 	case SDL_KEYDOWN:
	// 	{
	// 		switch(event->key.keysym.sym)
	// 		{
	// 			case SDLK_ESCAPE:
	// 			{
	// 				controls->quit = true;
	// 			} break;

	// 			case SDLK_UP:
	// 			{
	// 				controls->moveUp = true;	
	// 			} break;

	// 			case SDLK_DOWN:
	// 			{
	// 				controls->moveDown = true;	
	// 			} break;

	// 			case SDLK_LEFT:
	// 			{
	// 				controls->moveLeft = true;	
	// 			} break;

	// 			case SDLK_RIGHT:
	// 			{
	// 				controls->moveRight = true;	
	// 			} break;

	// 			default:
	// 			{
					
	// 			} break;

	// 		}
	// 	}
	// }
}

static float
SDLGetSecondsElapsed(uint64_t OldCounter, uint64_t CurrentCounter)
{
	return ((float)(CurrentCounter - OldCounter) /
	        (float)(SDL_GetPerformanceFrequency()));
}

int
main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		uint64_t performanceCountFrequency = SDL_GetPerformanceFrequency();
		int gameUpdateHz = 60;
		float targetSecondsPerFrame = 1.0f / (float)gameUpdateHz;

		// TODO(Jonny):		I'd like to intentionalaly open the highest possibly OpenGL
		//					version available on the users machine. Not defining any
		//					context *seems* to do this. There may be a more failsafe
		//					and manual way, but for now this works.

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		                    SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_Window *window = NULL;
		window = SDL_CreateWindow("Pizza Man",
								  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								  800, 600,
								  SDL_WINDOW_SHOWN |
								  SDL_WINDOW_OPENGL);

		if (window != NULL)
		{
			SDL_GLContext glRenderContext = NULL;

			glRenderContext = SDL_GL_CreateContext(window);

			SDL_GL_MakeCurrent(window, glRenderContext);

			if (glRenderContext != NULL)
			{
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					// WARNING(Jonny): 	Failed to set VSync!
					// TODO(Jonny): 	Logging
				}

				glewExperimental = GL_TRUE;
				GLenum err = glewInit();

				if (err == GLEW_OK)
				{
					// TODO(Jonny):		Do a test here to make sure computer can AT LEAST
					//					access OpenGL 3.0. Otherwise, the game will not run.

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

					
					GameMemory gameMemory = {};
					gameMemory.storageSize = Megabytes(64);
					gameMemory.storage = mmap(0, gameMemory.storageSize,
					                          PROT_READ | PROT_WRITE,
					                          MAP_ANON | MAP_PRIVATE,
					                          -1, 0);

					if (gameMemory.storage)
					{
						uint64_t lastCounter = SDL_GetPerformanceFrequency();

						bool running = true;
						while (running == true)
						{
							Controls controls = {};

							SDL_Event sdlEvent;
							while (SDL_PollEvent(&sdlEvent)) 
							{
								if (sdlEvent.type == SDL_QUIT)
								{
									running = false;
								}

								SDLGetKeyboardInput(&sdlEvent, &controls);
							}				

							if (controls.quit == true)
							{
								running = false;
							}


							Assert(sizeof(GameState) < gameMemory.storageSize);
							GameState *gameState = (GameState*)gameMemory.storage;

							if (gameMemory.isInitialized == false)
							{
								gameMemory.isInitialized = true;
								initGame(gameState);
							}
							else
							{
								update(gameState, &controls);

								// TODO(Jonny):		Create some sort of test which may
								//					*skip* a render frame if the MS is
								//					too great
								draw(gameState);

								SDL_GL_SwapWindow(window);
							}

							
							if (SDLGetSecondsElapsed(lastCounter, SDL_GetPerformanceCounter()) < targetSecondsPerFrame)
							{
								int32_t timeToSleep = (uint32_t)((targetSecondsPerFrame - SDLGetSecondsElapsed(lastCounter, SDL_GetPerformanceCounter())) * 1000) - 1;
								if (timeToSleep > 0)
								{
									SDL_Delay(timeToSleep);
								}
							}
							
							double endCounter = (double)SDL_GetPerformanceCounter();
							double counterElapsed = endCounter - lastCounter;
							double msPerFrame = (((1000.0f * (double)counterElapsed) / 
							                     (double)performanceCountFrequency));

							lastCounter = (uint64_t)endCounter;
						}
					}
					else
					{
						// OutputErrorMessage("Failed to Allocate Sufficient Memory!");
					}
				}
				else
				{
					// OutputErrorMessage((char*)glewGetErrorString(err));	
				}
			}
			else
			{
				// OutputErrorMessage(SDL_GetError());
			}

			// TODO(Jonny):	Delete the OpenGL Render Context somewhere about here
		}
		else
		{
			// OutputErrorMessage(SDL_GetError());
		}
		
		SDL_DestroyWindow(window);
	}
	else
	{
		// OutputErrorMessage(SDL_GetError());
	}

	SDL_Quit();

	return 0;
}
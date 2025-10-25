#pragma once


#include <memory>
#include "SDL2/include/SDL.h"
#include "Player/PlayerShip.h"

const int FPS = 60;
const float MIL_SEC_FRAME = 1000.0f / FPS;

class Game
{
private:
	
	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>myWindow;
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>myRenderer;
	std::unique_ptr<PlayerShip>myPlayer;

	Uint32 Ticks;
	float MIL_SEC_PREVIOUS_FRAME = 0;
	float DeltaTime = 0;

public:
	Game();
   ~Game();

   bool IsRunning;
   int WindWidth;
   int WindHeight;

   void InitGame();
   void ProcessInput();
   void Update();
   void Run();
   void Render();
   void Destroy();
};


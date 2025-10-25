#include "Game.h"


Game::Game():myWindow(nullptr,SDL_DestroyWindow),myRenderer(nullptr,SDL_DestroyRenderer),myPlayer(nullptr), WindWidth(800), WindHeight(300)
{
	IsRunning = false;
}

Game::~Game()
{
	Destroy();
}

void Game::InitGame()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	WindWidth = displayMode.w;
	WindHeight = displayMode.h;

	myWindow.reset(SDL_CreateWindow(("G1_GAME_ENGINE"), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,WindWidth,WindHeight,SDL_WINDOW_BORDERLESS));
	if (myWindow == nullptr)
	{
		return;
	}
	myRenderer.reset(SDL_CreateRenderer(myWindow.get(), 0, 0));
	if (myRenderer == nullptr)
	{
		return;
	}

	myPlayer.reset(new PlayerShip());
	myPlayer->Load(myRenderer.get(), "./images/SpaceShip.bmp");
	myPlayer->SetRectValue(200, 500, 100, 100);

	IsRunning = true;
}

void Game::ProcessInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		switch(evnt.type)
		{
		    case SDL_QUIT:
				IsRunning = false;
			break;

			case SDL_KEYDOWN:
				if (evnt.key.keysym.sym == SDLK_ESCAPE)
				{
					IsRunning = false;
				}
				if (myPlayer != nullptr)
				{
					myPlayer->IsKeyDown(evnt.key.keysym.sym);
				}
			break;

			case SDL_KEYUP:
				if (myPlayer != nullptr)
				{
					myPlayer->IsKeyUp(evnt.key.keysym.sym);
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				if (myPlayer != nullptr)
				{
					myPlayer->SetupBullet(myRenderer.get());
				}
			break;
		}
	}
}

void Game::Update()
{
	Uint32 TimeToDelay = MIL_SEC_FRAME - (SDL_GetTicks() - MIL_SEC_PREVIOUS_FRAME);
	if (TimeToDelay > 0 && TimeToDelay <= MIL_SEC_FRAME)
	{
		SDL_Delay(TimeToDelay);
	}

	Ticks = SDL_GetTicks();
	DeltaTime = (Ticks - MIL_SEC_PREVIOUS_FRAME) / 1000;
	MIL_SEC_PREVIOUS_FRAME = Ticks;

	if (myPlayer != nullptr)
	{
		myPlayer->Update(DeltaTime, WindWidth, WindHeight);
		myPlayer->FireBullets(DeltaTime);
	}
}

void Game::Run()
{
	while (IsRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Render()
{
	if (myRenderer != nullptr)
	{
		SDL_SetRenderDrawColor(myRenderer.get(), 21, 21, 21, 255);
		SDL_RenderClear(myRenderer.get());
		if (myPlayer != nullptr)
		{
			myPlayer->Render(myRenderer.get());
		}
		SDL_RenderPresent(myRenderer.get());
	}
}

void Game::Destroy()
{
	SDL_Quit();
}

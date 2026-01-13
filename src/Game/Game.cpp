#include "Game.h"


Game::Game():myWindow(nullptr,SDL_DestroyWindow),myRenderer(nullptr,SDL_DestroyRenderer), Galaxy(nullptr), Galaxy2(nullptr),
myPlayer(nullptr), WindWidth(800), WindHeight(300),Ticks(0),enemySpawner(nullptr), Score_M(nullptr), GameOver_M(nullptr)
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
	
	myRenderer.reset(SDL_CreateRenderer(myWindow.get(), 0, SDL_RENDERER_ACCELERATED));
	if (myRenderer == nullptr)
	{
		return;
	}
	
	Galaxy.reset(new GameEntity(myRenderer.get(), "./images/StarGal.bmp"));
	if (Galaxy == nullptr)
	{
		return;
	}
	Galaxy->SetRectValue(0, 0, WindWidth, WindHeight);

	Galaxy2.reset(new GameEntity(myRenderer.get(), "./images/StarGal.bmp"));
	if (Galaxy2 == nullptr)
	{
		return;
	}
	Galaxy2->SetRectValue(-1919, 0, WindWidth, WindHeight);

	myPlayer.reset(new PlayerShip());
	if (myPlayer != nullptr && myPlayer->IsPlayerAlive == true)
	{
		myPlayer->Load(myRenderer.get(), "./images/m1.bmp");
		myPlayer->SetRectValue(200, 500, Size, Size);
	}

	Score_M.reset(new ScoreManager("./font/Alan_Sans/static/AlanSans-Regular.ttf", FontSize));
	if (Score_M != nullptr)
	{
		Score_M->LoadFont(myRenderer.get(),Score);
		Score_M->SetRectValues(960,0, 35, 70);
	}

	GameOver_M.reset(new ScoreManager("./font/Alan_Sans/static/AlanSans-Regular.ttf", FontSize));
	if (GameOver_M != nullptr)
	{
		GameOver_M->LoadFont(myRenderer.get(), GameOver);
		GameOver_M->SetRectValues(700, 400, 500, 200);
	}


	enemySpawner.reset(new EnemySpawner());
	if (enemySpawner != nullptr)
	{
		enemySpawner->SetupEnemy(myRenderer.get(),WindWidth,WindHeight);
	}

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
					if (myPlayer->IsPlayerAlive == true)
					{
						myPlayer->IsKeyDown(evnt.key.keysym.sym);
					}
				}
			break;

			case SDL_KEYUP:
				if (myPlayer != nullptr)
				{
					if (myPlayer->IsPlayerAlive == true)
					{
						myPlayer->IsKeyUp(evnt.key.keysym.sym);
					}
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				if (myPlayer != nullptr)
				{
					if (myPlayer->IsPlayerAlive == true)
					{
						myPlayer->SetupBullet(myRenderer.get());
					}
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
	DeltaTime = (Ticks - MIL_SEC_PREVIOUS_FRAME) / 1000.f;
	MIL_SEC_PREVIOUS_FRAME = Ticks;

	if (myPlayer != nullptr)
	{
		if (myPlayer->IsPlayerAlive == true)
		{
			myPlayer->Update(DeltaTime, WindWidth, WindHeight);
			myPlayer->FireBullets(DeltaTime);
		}
	}

	if (enemySpawner != nullptr)
	{
		EnemySpawn();
	}

	BulletEnemyCollide();
	PlayerEnemyCollisionEnabled();
	Score_M->LoadFont(myRenderer.get(), Score);
	ParallaxScrolling();

}
void Game::EnemySpawn()
{
	if (enemySpawner != nullptr && myPlayer != nullptr && myPlayer->IsPlayerAlive == true)
	{
		enemySpawner->MoveEnemy(DeltaTime, myRenderer.get(),Score,WindWidth,WindHeight);
	}
}


bool Game::SetCollisionDetection(GameEntity* A, GameEntity* B)
{
	if (!A || !B || !A->GetTextureLoader() || !B->GetTextureLoader())
	{
		return false;
	}
	
	int Ax = A->GetTextureLoader()->GetPosX();
	int Ay = A->GetTextureLoader()->GetPosY();
	int Aw = A->GetTextureLoader()->GetWidth();
	int Ah = A->GetTextureLoader()->GetHeight();

	int Bx = B->GetTextureLoader()->GetPosX();
	int By = B->GetTextureLoader()->GetPosY();
	int Bw = B->GetTextureLoader()->GetWidth();
	int Bh = B->GetTextureLoader()->GetHeight();

	if (A != nullptr && B != nullptr)
	{
		if (Ax < Bx + Bw && Ax + Aw > Bx && Ay < By + Bh && Ay + Ah > By)
		{
			return true;
		}
	}
	return false;
}


void Game::PlayerEnemyCollisionEnabled()
{
	if (myPlayer != nullptr && enemySpawner != nullptr)
	{
		GameEntity* player = myPlayer->GetPlayerShip();

		GameEntity* bullet = myPlayer->GetBullet();

		for (auto enemy : enemySpawner->MyEnemy)
		{
			bool IsPlayerCollideWithEnemy = SetCollisionDetection(player, enemy);
			

			if (IsPlayerCollideWithEnemy == true)
			{
				myPlayer->IsPlayerAlive = false;
			}
		}
	}
}

void Game::BulletEnemyCollide()
{
	if (myPlayer != nullptr && enemySpawner != nullptr)
	{
		auto& enemies = enemySpawner->MyEnemy;

		for (int i = 0; i < enemies.size(); ++i)
		{
			GameEntity* enemy = enemies[i];

			if (enemy == nullptr)
				continue;

			for (int j = 0; j < myPlayer->Bulllets.size(); ++j)
			{
				GameEntity* bullet = myPlayer->Bulllets[j];

				if (bullet == nullptr)
					continue;

				if (bullet->IsColliding(enemy))
				{
	
					delete enemy;
					printf("Enemy Collide With Bullets");
					enemies.erase(enemies.begin() + i);
					--i;

					delete bullet;
					myPlayer->Bulllets.erase(myPlayer->Bulllets.begin() + j);
					--j;

					Score += 1;
					break;
				}
			}
		}
	}

}

void Game::ParallaxScrolling()
{
	if (myPlayer.get() == nullptr)
	{
		return;
	}
	if (myPlayer->IsPlayerAlive == true)
	{
		if (Galaxy != nullptr && Galaxy2 != nullptr)
		{
			Galaxy.get()->GetTextureLoader()->SetPosX(Galaxy1x += 4);
			Galaxy2.get()->GetTextureLoader()->SetPosX(Galaxy2x += 4);

			if (Galaxy.get()->GetTextureLoader()->GetPosX() > WindWidth)
			{
				Galaxy1x = -1919;
				std::cout << "Galaxy 1 reset scrolling" << std::endl;
			}
			if (Galaxy2.get()->GetTextureLoader()->GetPosX() > WindWidth)
			{
				Galaxy2x = -1919;
				std::cout << "Galaxy 2 reset scrolling" << std::endl;
			}
		}
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
		SDL_SetRenderDrawColor(myRenderer.get(), 0, 0, 0, 255);
		SDL_RenderClear(myRenderer.get());
		if (Galaxy != nullptr)
		{
			Galaxy->Render(myRenderer.get());
		}
		if (Galaxy2 != nullptr)
		{
			Galaxy2->Render(myRenderer.get());
		}
		if (myPlayer != nullptr)
		{
			if (myPlayer->IsPlayerAlive == true)
			{
				myPlayer->Render(myRenderer.get());
			}
		}
		
		if (Score_M != nullptr)
		{
			Score_M->Render(myRenderer.get());
		}
		if (myPlayer->IsPlayerAlive == false && GameOver_M != nullptr)
		{
			GameOver_M->Render(myRenderer.get());
		}
		if (enemySpawner != nullptr)
		{
			enemySpawner->Render(myRenderer.get());
		}
		
		SDL_RenderPresent(myRenderer.get());
	}
}

void Game::Destroy()
{
	SDL_Quit();
}

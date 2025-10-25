#include "Game.h"


Game::Game():myWindow(nullptr,SDL_DestroyWindow),myRenderer(nullptr,SDL_DestroyRenderer),myPlayer(nullptr), WindWidth(800), WindHeight(300),Ticks(0),enemySpawner(nullptr)
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
	if (myPlayer != nullptr && myPlayer->IsPlayerAlive == true)
	{
		myPlayer->Load(myRenderer.get(), "./images/SpaceShip.bmp");
		myPlayer->SetRectValue(200, 500, 100, 100);
	}
	enemySpawner.reset(new EnemySpawner());
	enemySpawner->SetupEnemy(myRenderer.get());

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
	DeltaTime = (Ticks - MIL_SEC_PREVIOUS_FRAME) / 1000;
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
	

}
void Game::EnemySpawn()
{
	if (enemySpawner != nullptr)
	{
		enemySpawner->MoveEnemy(DeltaTime, myRenderer.get());
		SDL_Delay(5);
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
			auto& enemy = enemies[i];

			if (enemy == nullptr)
				continue;

			for (int j = 0; j < myPlayer->Bulllets.size(); ++j)
			{
				auto& bullet = myPlayer->Bulllets[j];
				if (bullet == nullptr)
					continue;

				if (bullet->IsColliding(enemy))
				{
					printf("Enemy Collide With Bullets");
					enemies.erase(enemies.begin() + i);
					--i;

					delete bullet;
					myPlayer->Bulllets.erase(myPlayer->Bulllets.begin() + j);
					--j;
					break;
				}
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
		SDL_SetRenderDrawColor(myRenderer.get(), 21, 21, 21, 255);
		SDL_RenderClear(myRenderer.get());
		if (myPlayer != nullptr)
		{
			if (myPlayer->IsPlayerAlive == true)
			{
				myPlayer->Render(myRenderer.get());
			}
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

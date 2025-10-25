#include "EnemySpawner.h"

EnemySpawner::EnemySpawner()
{
	
}
EnemySpawner::~EnemySpawner()
{

}
void EnemySpawner::SetupEnemy(SDL_Renderer* myRenderer,int WindWidth, int WindHeight)
{
	GameEntity* enemy = new GameEntity(myRenderer,"./images/EvilShip.bmp");

	if (enemy != nullptr)
	{
		int x = WindWidth;
		int y = std::rand() % (WindHeight - EnemySize);
		enemy->GetTextureLoader()->SetRectValues(x, y, EnemySize, EnemySize);
		MyEnemy.push_back(enemy);
	}
	printf("Enemy spawned! Total enemies: %zu\n", MyEnemy.size());
}
void EnemySpawner::Render(SDL_Renderer* myRen)
{
	for (auto enemy : MyEnemy)
	{
		enemy->Render(myRen);
	}
}
void EnemySpawner::MoveEnemy(float DeltaTime,SDL_Renderer* myRen,unsigned int& Score)
{
	SpawnTime += DeltaTime;

	if (SpawnTime >= SpawnInterval)
	{
		SetupEnemy(myRen);
		SpawnTime = 0.0f;
	}

	for (int i = 0; i < MyEnemy.size(); ++i)
	{
		auto enemy = MyEnemy[i];
		if (enemy != nullptr)
		{
			int x = enemy->GetTextureLoader()->GetPosX();
			if (x + EnemySize > 0)
			{
				x -= static_cast<int>(EnemySpeed * DeltaTime);
				enemy->GetTextureLoader()->SetPosX(x);
			}
			else if(x + EnemySize < 0)
			{
				if (Score <= 0)
				{
					Score = 0;
				}
				else 
				{
					Score -= 1;
				}
				
				printf("Enemy removed! Total enemies before removal: %zu\n", MyEnemy.size());
				MyEnemy.erase(MyEnemy.begin() + i);
				--i;
			}
		}
	}
}
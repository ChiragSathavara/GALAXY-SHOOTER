#pragma once
#include <string>
#include <vector>
#include "Entity/GameEntity.h"
#include "SDL2/include/SDL.h"

class EnemySpawner
{
public:
	EnemySpawner();
   ~EnemySpawner();

   void SetupEnemy(SDL_Renderer* myRenderer);
   void MoveEnemy(float DeltaTime,SDL_Renderer* myRen, unsigned int& Score);
   void Render(SDL_Renderer* myRen);
   

   std::vector<GameEntity*>MyEnemy;

   

private:
	
	
	int EnemySize = 100;
	int EnemySpeed = 400;
	float SpawnTime = 0;
	float SpawnInterval = 2;
};


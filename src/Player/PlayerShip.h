#pragma once
#include "Entity/GameEntity.h"
#include "SDL2/include/SDL.h"
#include <memory>

class PlayerShip
{
public:
	
	PlayerShip();
   ~PlayerShip();

   bool IsPlayerAlive;

   void Load(SDL_Renderer* myRen, std::string address);
   void SetRectValue(int x, int y, int w, int h);
   void Render(SDL_Renderer* myRen);
   
   void IsKeyUp(SDL_Keycode& Key);
   void IsKeyDown(SDL_Keycode& Key);

   void Update(float DeltaTime, int WindWidth, int WindHeight);

   void PlayerMovement(float DeltaTime);
   
   void SetupBullet(SDL_Renderer* myRen);

   void FireBullets(float DeltaTime);

   GameEntity* GetBullet();

   GameEntity* GetPlayerShip();

   std::vector<GameEntity*> Bulllets;

private:

	std::unique_ptr<GameEntity>myPlayer;

	
	
	bool MoveUp, MoveDown , MoveRight, MoveLeft;
	
	int PlayerPosX = 200;
	int PlayerPosY = 500;
	int PlayerSize = 100;
	int PlayerSpeed = 200;
	int BulletSpeed = 1000;
	int SpawnBulletPosX = 80;
	int SpawnBulletPosY = 35;

	int BulletSize = 22;

	int windwidth = 0;
};


#include "PlayerShip.h"

PlayerShip::PlayerShip():myPlayer(nullptr)
{
	MoveUp = false;
	MoveDown = false;
	MoveRight = false;
	MoveLeft = false;
	IsPlayerAlive = true;
	
}

PlayerShip::~PlayerShip()
{

}
void PlayerShip::IsKeyUp(SDL_Keycode& Key)
{
	if (IsPlayerAlive == true)
	{
		switch (Key)
		{
		case SDLK_w: MoveUp = false; break;
		case SDLK_s: MoveDown = false; break;
		case SDLK_d: MoveRight = false; break;
		case SDLK_a: MoveLeft = false; break;
		}
	}
}
void PlayerShip::IsKeyDown(SDL_Keycode& Key)
{
	if (IsPlayerAlive == true)
	{
		switch (Key)
		{
		case SDLK_w: MoveUp = true; break;
		case SDLK_s: MoveDown = true; break;
		case SDLK_d: MoveRight = true; break;
		case SDLK_a: MoveLeft = true; break;
		}
	}
}
void PlayerShip::Update(float DeltaTime, int WindWidth, int WindHeight)
{
	if (IsPlayerAlive == true)
	{
		windwidth = WindWidth;

		PlayerMovement(DeltaTime);
		if (PlayerPosX < 0) { PlayerPosX = 0; }
		if (PlayerPosY < 0) { PlayerPosY = 0; }
		if (PlayerPosX + PlayerSize > WindWidth) { PlayerPosX = WindWidth - PlayerSize; }
		if (PlayerPosY + PlayerSize > WindHeight) { PlayerPosY = WindHeight - PlayerSize; }

		myPlayer->SetRectValue(PlayerPosX, PlayerPosY, 100, 100);
	}
	
}
void PlayerShip::PlayerMovement(float DeltaTime)
{
	if (IsPlayerAlive == true)
	{
		if (MoveUp == true) { PlayerPosY -= PlayerSpeed * DeltaTime; }
		if (MoveDown == true) { PlayerPosY += PlayerSpeed * DeltaTime; }
		if (MoveRight == true) { PlayerPosX += PlayerSpeed * DeltaTime; }
		if (MoveLeft == true) { PlayerPosX -= PlayerSpeed * DeltaTime; }
	}
}

void PlayerShip::SetupBullet(SDL_Renderer* myRen)
{
	if (IsPlayerAlive == true)
	{
		GameEntity* bullet = new GameEntity(myRen, "./images/kamal.bmp");

		int pX = PlayerPosX + SpawnBulletPosX;
		int pY = PlayerPosY + SpawnBulletPosY;

		bullet->SetRectValue(pX, pY, BulletSize, BulletSize);

		Bulllets.push_back(bullet);

		std::cout << "Bullet Counter :- " << Bulllets.size() << std::endl;
	}
}

void PlayerShip::FireBullets(float DeltaTime)
{
	if (IsPlayerAlive == true)
	{
		for (int i = 0; i < Bulllets.size(); ++i)
		{
			GameEntity* bullet = Bulllets[i];
			int px = bullet->GetTextureLoader()->GetPosX();
			px += static_cast<int>(BulletSpeed * DeltaTime);
			bullet->GetTextureLoader()->SetPosX(px);


			if (px > windwidth)
			{
				delete bullet;
				Bulllets.erase(Bulllets.begin() + i);
				i--;
			}
		}
	}
}
GameEntity* PlayerShip::GetPlayerShip()
{
	return myPlayer.get();
}
void PlayerShip::Load(SDL_Renderer* myRen, std::string address)
{
	myPlayer.reset(new GameEntity(myRen, address));
}

void PlayerShip::SetRectValue(int x, int y, int w, int h)
{
	if (IsPlayerAlive == true)
	{
		myPlayer->SetRectValue(x, y, w, h);
	}
}

void PlayerShip::Render(SDL_Renderer* myRen)
{
	if (IsPlayerAlive == true)
	{
		myPlayer->Render(myRen);
		for (auto myBullet : Bulllets)
		{
			if (myBullet != nullptr)
			{
				myBullet->Render(myRen);
			}
		}
	}
}
GameEntity* PlayerShip::GetBullet()
{
	for (auto mybullet : Bulllets)
	{
		return mybullet;
	}
}
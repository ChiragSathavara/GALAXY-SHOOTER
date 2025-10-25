#pragma once

#include <string>
#include "SDL2/include/SDL.h"
#include "Loader/Texture/TextureLoader.h"

class GameEntity
{
public:
	GameEntity(SDL_Renderer* myRen, std::string address);
   ~GameEntity();

   void SetRectValue(int x, int y, int w, int h);
   
   TextureLoader* GetTextureLoader();
   void Render(SDL_Renderer* myRen);
   bool IsColliding(GameEntity* Obj);

private:

	std::unique_ptr<TextureLoader>my_Texture;
};


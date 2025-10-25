#pragma once

#include <string>
#include "SDL2/include/SDL.h"
#include "Manager/Surface/SurfaceManager.h"

class TextureLoader
{
public:
	TextureLoader(SDL_Renderer* myRen, std::string Address);
   ~TextureLoader();

   void Load(SDL_Renderer* myRen, std::string Address);
   void SetRectValues(int x, int y, int w, int h);
   
   void SetPosX(int X);
   void SetPosY(int Y);

   int GetPosX();
   int GetPosY();
   int GetWidth();
   int GetHeight();

   SDL_Rect& GetRect();

   void Render(SDL_Renderer* myRen);
private:

	SDL_Surface* mySurface = nullptr;
	SDL_Texture* myTexture = nullptr;

	SDL_Rect myRect;
};


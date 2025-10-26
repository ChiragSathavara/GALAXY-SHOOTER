#pragma once

#include <string>
#include <memory>
#include "SDL_ttf.h"

class ScoreManager
{
public:
	ScoreManager(std::string Address, int& fontSize);
   ~ScoreManager();

   void LoadFont(SDL_Renderer* myRen, unsigned int& Score);
   void LoadFont(SDL_Renderer* myRen, std::string Text);
   void SetRectValues(int x, int y, int Width, int Height);
   void Render(SDL_Renderer* myRen);
private:

	TTF_Font* OurFont;
	SDL_Rect myrect;
	std::unique_ptr<SDL_Surface,void(*)(SDL_Surface*)>OurSurface;
	std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>OurTexture;

};


#include "ScoreManager.h"
#include "SDL2/include/SDL.h"

ScoreManager::ScoreManager(std::string Address, int& fontSize):OurSurface(nullptr,SDL_FreeSurface), OurTexture(nullptr,SDL_DestroyTexture)
{
	if (TTF_Init() == -1)
	{
		printf("TTF isnt Initialize!");
	}
	else
	{
		printf("TTF is Initialize!");
	}
	OurFont = TTF_OpenFont(Address.c_str(), fontSize);
	if (OurFont == nullptr)
	{
		printf("FONT NOT LOAD");
		return;
	}

}

ScoreManager::~ScoreManager()
{
	TTF_CloseFont(OurFont);
}

void ScoreManager::LoadFont(SDL_Renderer* myRen, unsigned int& Score)
{
	//./font/Alan_Sans/static/AlanSans-Black.ttf
	//font size 32
	
	std::string myText = std::to_string(Score);

	OurSurface.reset((TTF_RenderText_Solid(OurFont, myText.c_str(), { 255,255,255 })));
	if (OurSurface == nullptr)
	{
		return;
	}

	OurTexture.reset((SDL_CreateTextureFromSurface(myRen,OurSurface.get())));
	if (OurTexture == nullptr)
	{
		return;
	}

}
void ScoreManager::SetRectValues(int x, int y, int Width, int Height)
{
	myrect.x = x;
	myrect.y = y;
	myrect.w = Width;
	myrect.h = Height;
}
void ScoreManager::Render(SDL_Renderer* myRen)
{
	SDL_RenderCopy(myRen, OurTexture.get(), NULL, &myrect);
}

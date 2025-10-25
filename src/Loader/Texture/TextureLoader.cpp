#include "TextureLoader.h"

TextureLoader::TextureLoader(SDL_Renderer* myRen, std::string Address)
{
	Load(myRen, Address);
}

TextureLoader::~TextureLoader()
{
	SDL_DestroyTexture(myTexture);
}
void TextureLoader::Load(SDL_Renderer* myRen, std::string Address)
{
	mySurface = SurfaceManager::GetInstance()->GetSurface(Address);

	if (mySurface == nullptr)
	{
		return;
	}

	myTexture = SDL_CreateTextureFromSurface(myRen, mySurface);

	if (myTexture == nullptr)
	{
		return;
	}
}

void TextureLoader::SetRectValues(int x, int y, int w, int h)
{
	myRect.x = x;
	myRect.y = y;
	myRect.w = w;
	myRect.h = h;
}
SDL_Rect& TextureLoader::GetRect()
{
	return  myRect;
}
int TextureLoader::GetPosX()
{
	return myRect.x;
}

int TextureLoader::GetPosY()
{
	return myRect.y;
}
void TextureLoader::SetPosX(int X)
{
	myRect.x = X;
}
void TextureLoader::SetPosY(int Y)
{
	myRect.y = Y;
}
int  TextureLoader::GetWidth()
{
	return myRect.w;
}
int  TextureLoader::GetHeight()
{
	return myRect.h;
}
void TextureLoader::Render(SDL_Renderer* myRen)
{
	SDL_RenderCopy(myRen, myTexture, NULL, &myRect);
}

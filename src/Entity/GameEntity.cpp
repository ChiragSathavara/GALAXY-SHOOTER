#include "GameEntity.h"

GameEntity::GameEntity(SDL_Renderer* myRen, std::string address)
{
	my_Texture.reset(new TextureLoader(myRen, address));
}

GameEntity::~GameEntity()
{

}
void GameEntity::SetRectValue(int x, int y, int w, int h)
{
	my_Texture->SetRectValues(x, y, w, h);
}
TextureLoader* GameEntity::GetTextureLoader()
{
	return my_Texture.get();
}

void GameEntity::Render(SDL_Renderer* myRen)
{
	if (my_Texture != nullptr)
	{
		my_Texture->Render(myRen);
	}
}
#include "SurfaceManager.h"

SurfaceManager* SurfaceManager::GetInstance()
{
	static SurfaceManager mySurface;
	return &mySurface;
}

SDL_Surface* SurfaceManager::GetSurface(std::string address)
{
	auto Search = m_surface.find(address);

	if (Search != m_surface.end())
	{
		return m_surface[address];
	}
	else
	{
		SDL_Surface* mySurface = SDL_LoadBMP(address.c_str());
		m_surface.insert(std::make_pair(address.c_str(), mySurface));
		return m_surface[address];
	}

	return nullptr;
}

#pragma once
#include <iostream>
#include <unordered_map>
#include "SDL2/include/SDL.h"

class SurfaceManager
{
private:
	SurfaceManager() = default;
	~SurfaceManager() = default;

	SurfaceManager(const SurfaceManager&) = delete;
	SurfaceManager& operator = (const SurfaceManager&) = delete;

public:

	static SurfaceManager* GetInstance();
	SDL_Surface* GetSurface(std::string address);

	std::unordered_map<std::string, SDL_Surface*>m_surface;
};


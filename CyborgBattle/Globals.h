#pragma once
#include <string>
#include <iostream>
#include <SDL.h>
#include "randomNumber.h"

class Entity;
enum class Direction : int;

class Globals
{
public:
	// math
	static const float PI;

	// tools
	static bool debugging;

	// SDL
	static int screenWidth, screenHeight, screenScale;
	static SDL_Renderer* renderer;

	// removes header from lines (e.g: removes "clip: ")
	static std::string clipOffDataHeader(const std::string& data);

	// helper functions
	static float		distanceBetweenTwoRects(const SDL_Rect& r1, const SDL_Rect& r2);
	static float		distanceBetweenTwoEntities(Entity* e1, Entity* e2);
	static float		angleBetweenTwoEntities(Entity* e1, Entity* e2);
	static bool			checkCollision(SDL_Rect cbox1, SDL_Rect cbox2);
	static Direction    angleToDirection(float angle);
	static float		angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
	static float		angleBetweenTwoRects(const SDL_Rect& r1, const SDL_Rect& r2);
};


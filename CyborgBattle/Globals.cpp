#include "Globals.h"
#include "Entity.h"

const float Globals::PI = 3.14159f;

bool Globals::debugging = false;

int Globals::screenWidth = 640;
int Globals::screenHeight = 352;
int Globals::screenScale = 2;
SDL_Renderer* Globals::renderer = nullptr;

std::string Globals::clipOffDataHeader(const std::string& data)
{
	std::string dataCopy = data;
	std::size_t pos = data.find(":", 0);
	if (pos != -1)
	{
		dataCopy = data.substr(pos + 1, data.length() - pos + 2);
	}
	return dataCopy;
}

float Globals::distanceBetweenTwoRects(const SDL_Rect& r1, const SDL_Rect& r2)
{
	SDL_Point p1, p2;
	p1.x = r1.x + r1.w / 2;
	p1.y = r1.x + r1.h / 2;

	p2.x = r2.x + r2.w / 2;
	p2.y = r2.y + r2.h / 2;

	// compute distance between the centers
	float d = std::abs(std::sqrt(
		std::pow((p2.x - p1.x), 2) + std::pow((p2.y - p1.y), 2)));
	return d;
}

float Globals::distanceBetweenTwoEntities(Entity* e1, Entity* e2)
{
	if (!e1 || !e2)
		return 0;
	float d = std::abs(std::sqrt(
		std::pow((e2->getX() - e1->getX()), 2) + std::pow((e2->getY() - e1->getY()), 2)));
	return d;
}

float Globals::angleBetweenTwoEntities(Entity* e1, Entity* e2)
{
	float dx, dy;
	float x1 = e1->getX();
	float y1 = e1->getY();
	float x2 = e2->getX();
	float y2 = e2->getY();

	dx = x2 - x1;
	dy = y2 - y1;

	// return in degrees
	return std::atan2(dy, dx) * 180 / Globals::PI;
}

bool Globals::checkCollision(SDL_Rect cbox1, SDL_Rect cbox2)
{
	SDL_Rect intersection;
	if (SDL_IntersectRect(&cbox1, &cbox2, &intersection))
	{
		return true;
	}

	// OPTIONAL TODO
	// if a rectangle is in another rectangle

	return false;
}

Direction Globals::angleToDirection(float angle)
{
	if ((angle >= 0 && angle <= 45) || (angle >= 315 && angle <= 360))
	{
		return Direction::RIGHT;
	} // subsequently...90 degree sections
	else if (angle > 45 && angle <= 135)
	{
		return Direction::DOWN;
	}
	else if (angle > 135 && angle <= 225)
	{
		return Direction::LEFT;
	}
	
	return Direction::UP;
}

float Globals::angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2)
{
	float dx = cx2 - cx1;
	float dy = cy2 - cy1;

	return std::atan2(dy, dx) * 180 / Globals::PI;
}

float Globals::angleBetweenTwoRects(const SDL_Rect& r1, const SDL_Rect& r2)
{
	float cx1 = r1.x + (r1.w / 2.0f);
	float cy1 = r1.y + (r1.h / 2.0f);
							   
	float cx2 = r2.x + (r2.w / 2.0f);
	float cy2 = r2.y + (r2.h / 2.0f);
	
	return angleBetweenTwoPoints(cx1, cy1, cx2, cy2);
}

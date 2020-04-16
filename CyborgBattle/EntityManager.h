#pragma once
#include <list>
#include "Entity.h"


class EntityManager
{
public:
	~EntityManager() {};
	static EntityManager& getInstance();

	// depth compare
	static bool EntityCompare(const Entity* const &a, const Entity* const &b);
	void removeInactiveEntities(std::list<Entity*>& entityList, bool deleteEntities);
	void removeInactiveEntities(bool deleteEntities);
	void removeAll(std::list<Entity*>& entityList, bool deleteEntities);
	void removeAll(bool deleteEntities);
	void addEntity(Entity* entity);
	void sortEntities();
	const std::list<Entity*>& getEntities();
private:
	EntityManager() {}

	std::list<Entity*> entities;
};


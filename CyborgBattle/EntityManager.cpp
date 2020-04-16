#include "EntityManager.h"

EntityManager& EntityManager::getInstance()
{
	// static local var, object only initialized once, but scope restricted
	static EntityManager instance;
	return instance;
}

bool EntityManager::EntityCompare(const Entity* const &a, const Entity* const &b)
{
	if (!a || !b)
		return false;

	return a->getY() < b->getY();
}

void EntityManager::removeInactiveEntities(std::list<Entity*>& entityList, bool deleteEntities)
{
	for (auto it = entityList.begin(); it != entityList.end();)
	{
		if (!*it)
		{
			it = entityList.erase(it);
			continue;
		}

		if (!((*it)->isActive()))
		{
			if (deleteEntities)
				delete *it;
			it = entityList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void EntityManager::removeInactiveEntities(bool deleteEntities)
{
	removeInactiveEntities(this->entities, deleteEntities);
}

void EntityManager::removeAll(std::list<Entity*>& entityList, bool deleteEntities)
{
	for (auto it = entityList.begin(); it != entityList.end();)
	{
		if (deleteEntities)
			delete *it;
		it = entityList.erase(it);
	}
}

void EntityManager::removeAll(bool deleteEntities)
{
	removeAll(this->entities, deleteEntities);
}

void EntityManager::addEntity(Entity* entity)
{
	if (!entity)
		return;
	this->entities.push_back(entity);
}

void EntityManager::sortEntities()
{
	this->entities.sort(&EntityManager::EntityCompare);
}

const std::list<Entity*>& EntityManager::getEntities()
{
	return this->entities;
}

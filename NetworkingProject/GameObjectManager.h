#pragma once
#include "SpriteObject.h"

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Add(std::string name, SpriteObject* gameObject);
	void Remove(std::string name);
	int GetObjectCount() const;
	SpriteObject* Get(std::string name) const;

	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll();
	void SetPreviousPosition(sf::String id);
	void SetPreviousPositionAll();
	void UpdateObject(sf::String id);
private:
	std::map<std::string, SpriteObject*> _gameObjects;
	sf::Clock clock;

	struct GameObjectDeallocator
	{
		void operator()(const std::pair<std::string, SpriteObject*> & p) const
		{
			delete p.second;
		}
	};
};
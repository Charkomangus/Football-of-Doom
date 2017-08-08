/*********************************************************************************
* Manager/Pool for sprite objects - Keeps things neeter in game manager
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/

#pragma once
#include "SpriteObject.h"

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Add(std::string name, SpriteObject* gameObject);
	void Remove(std::string name);
	void RemoveAll();
	int GetObjectCount() const;
	SpriteObject* Get(std::string name) const;
	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll();
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
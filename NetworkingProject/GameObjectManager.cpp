/*********************************************************************************
* Manager/Pool for sprite objects - Keeps things neeter in game manager
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/

#include "stdafx.h"
#include "GameObjectManager.h"
#include "GameManager.h"


GameObjectManager::GameObjectManager()
{
}

//Destroy all game objects
GameObjectManager::~GameObjectManager()
{
	for_each(_gameObjects.begin(), _gameObjects.end(), GameObjectDeallocator());
}

//Add a new game object in the map, using a name given as it's key
void GameObjectManager::Add(std::string name, SpriteObject* gameObject)
{
	_gameObjects.insert(std::pair<std::string, SpriteObject*>(name, gameObject));
}

//Go through the map of game objects and delete the one whose key matches the value given
void GameObjectManager::Remove(std::string name)
{
	std::map<std::string, SpriteObject*>::iterator results = _gameObjects.find(name);
	if (results != _gameObjects.end())
	{
		delete results->second;
		_gameObjects.erase(results);
	}
}

//Update all the objects
void GameObjectManager::RemoveAll()
{
	std::map<std::string, SpriteObject*>::const_iterator itr = _gameObjects.begin();
	while (itr != _gameObjects.end())
	{
		delete itr->second;	

		++itr;
	}
}

//Go through the map of game objects and return the result whose key matches the value given
SpriteObject* GameObjectManager::Get(std::string name) const
{
	std::map<std::string, SpriteObject*>::const_iterator results = _gameObjects.find(name);
	if (results == _gameObjects.end())
		return nullptr;
	return results->second;
}

//Return the map size
int GameObjectManager::GetObjectCount() const
{
	return _gameObjects.size();
}

//Update all the objects
void GameObjectManager::UpdateAll()
{
	std::map<std::string, SpriteObject*>::const_iterator itr = _gameObjects.begin();
	float timeDelta = clock.restart().asSeconds();

	while (itr != _gameObjects.end())
	{		
		itr->second->Update(timeDelta);

		++itr;
	}
}

//Update a particular object
void GameObjectManager::UpdateObject(sf::String id)
{
	SpriteObject* object = Get(id);
	float timeDelta = clock.restart().asSeconds();
	object->Update(timeDelta);
}

//Draw all the game objects
void GameObjectManager::DrawAll(sf::RenderWindow& renderWindow)
{
	std::map<std::string, SpriteObject*>::const_iterator itr = _gameObjects.begin();
	while (itr != _gameObjects.end())
	{
		itr->second->Draw(renderWindow);
		++itr;
	}
}
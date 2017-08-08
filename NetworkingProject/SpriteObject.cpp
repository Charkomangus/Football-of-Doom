/*********************************************************************************
* Sprite Object - the default game object, most of it's values can be set. 
* Contains interpolation movement code for objects.
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/


#include "stdafx.h"
#include "SpriteObject.h"
#include "GameManager.h"


//Defaultly the sprite object is not loaded
SpriteObject::SpriteObject(): _isLoaded(false)
{
}

SpriteObject::~SpriteObject()
{
}

//Virtual method
void SpriteObject::Update(float elapsedTime)
{
}

//Move using input
void SpriteObject::ReceiveInput(sf::Vector2f _velocity)
{
}

//Interpolate movement using networked information
void SpriteObject::InterpolateMovement()
{
	//Only interpolate when there are 3 position packets - minumum amount of information for linear interpolation

	

	if (positionsCache.size() < 3) return;

	//Lerp jump percentage
	float percentage = 0.5f;

	//Save copies of the positional data
	PositionStamp first = positionsCache.back();
	PositionStamp third = positionsCache.front();

	//Remove the oldest packet from the list - this makes room for new positional information
	positionsCache.pop_front();
	PositionStamp second = positionsCache.front();

	//Use the 3 positional points to create two samples of velocity
	sf::Vector2f oldVelocity = ReturnVelocity(third, second);
	sf::Vector2f newVelocity = ReturnVelocity(second, first);

	//Interpolate between the two samples of velocity
	sf::Vector2f interpolatedVelocity;
	interpolatedVelocity.x = (newVelocity.x * (1.0 - percentage) + oldVelocity.x * percentage) / 30.0f;
	interpolatedVelocity.y = (newVelocity.y * (1.0 - percentage) + oldVelocity.y * percentage) / 30.0f;

	//Move this object by the new interpolated velocity
	GetSprite().move(interpolatedVelocity);

}

//Finds the distance between two points and then divides it by their timestamp. The resulting vector is returned
sf::Vector2f SpriteObject::ReturnVelocity(PositionStamp oldPosition, PositionStamp newPosition)
{
	float DistanceX = oldPosition.position.x - newPosition.position.x;
	float DistanceY = oldPosition.position.y - newPosition.position.y;
	float time = oldPosition.timestamp - newPosition.timestamp;
	return sf::Vector2f(DistanceX / time, DistanceY / time);
}


//Load the sprite object. If there is no texture do not initialize the object
void SpriteObject::Load(std::string filename)
{
	if (_image.loadFromFile(filename) == false)
	{
		_filename = "";
		_isLoaded = false;
	}
	else
	{
		_filename = filename;
		_sprite.setTexture(_image);
		_isLoaded = true;
	}
}

//Draw ther object on the screen
void SpriteObject::Draw(sf::RenderWindow & renderWindow)
{
	if (_isLoaded)
	{
		renderWindow.draw(_sprite);
	}
}

//Set the objects position
void SpriteObject::SetPosition(float x, float y)
{
	if (_isLoaded)
	{
		_sprite.setPosition(x, y);
	}
}

//Get the objects position
sf::Vector2f SpriteObject::GetPosition() const
{
	if (_isLoaded)
	{	
		return _sprite.getPosition();
	}
	return sf::Vector2f();
}

//Return the current sprite
sf::Sprite& SpriteObject::GetSprite()
{
	if (_isLoaded)
	{
		return _sprite;
	}
	return sf::Sprite();

}

//Check if the object has been loaded
bool SpriteObject::IsLoaded() const
{
	return _isLoaded;
}

//Helper function - return objects height
float SpriteObject::GetHeight() const
{
	if (_isLoaded)
	{
		return _sprite.getLocalBounds().height;
	}
	return -1.0f;
}

//Helper function - return objects rotation
float SpriteObject::GetRotation() const
{
	if (_isLoaded)
	{
		return _sprite.getRotation();
	}
	return -1.0f;
}

//Helper function - set objects rotation
void SpriteObject::SetRotation(float newRotation)
{
	if (_isLoaded)
	{
		_sprite.setRotation(newRotation);		
	}	
}


//Helper function - return objects width
float SpriteObject::GetWidth() const
{
	if (_isLoaded)
	{
		return _sprite.getLocalBounds().width;
	}
	return -1.0f;
}


//Return a bounding rect 
sf::Rect<float> SpriteObject::GetBoundingRect() const
{
	if (_isLoaded)
	{
		return _sprite.getGlobalBounds();
	}
	return sf::Rect<float>();
}


//Add a position to the position cache list
void SpriteObject::AddPositionToCache(double time, sf::Vector2f newPosition)
{
	PositionStamp temp;
	temp.timestamp = time;
	temp.position = newPosition;
	positionsCache.push_back(temp);
}

//Return the position cache list
std::list<PositionStamp> SpriteObject::ReturnPositionCache()
{
	return positionsCache;
}



//A visible object that can have its position and image set

#include "stdafx.h"
#include "SpriteObject.h"

//Defaultly the sprite object is not loaded
SpriteObject::SpriteObject(): _isLoaded(false)
{
}

SpriteObject::~SpriteObject()
{
}

void SpriteObject::Update(float elapsedTime)
{
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
	return _sprite;
}

//Check if the object has been loaded
bool SpriteObject::IsLoaded() const
{
	return _isLoaded;
}

//Helper function - return objects height
float SpriteObject::GetHeight() const
{
	return _sprite.getLocalBounds().height;
}

//Helper function - return objects width
float SpriteObject::GetWidth() const
{
	return _sprite.getLocalBounds().width;
}

//Return a bounding rect 
sf::Rect<float> SpriteObject::GetBoundingRect() const
{
	return _sprite.getGlobalBounds();
}
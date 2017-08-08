#pragma once
#include <array>


struct PositionStamp
{
	double timestamp;
	sf::Vector2f position;
};
/*********************************************************************************
* Sprite Object - the default game object, most of it's values can be set.
* Contains interpolation movement code for objects.
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/

class SpriteObject
{
public:
	SpriteObject();
	virtual ~SpriteObject();
	virtual bool IsLoaded() const;
	virtual void Load(std::string filename);
	virtual void Draw(sf::RenderWindow & window);
	virtual void Update(float elapsedTime);	
	virtual void ReceiveInput(sf::Vector2f _velocity);

	virtual void SetPosition(float x, float y);
	virtual sf::Vector2f GetPosition() const;
	virtual float GetWidth() const;
	virtual float GetHeight() const;
	virtual float GetRotation() const;
	virtual void SetRotation(float newRotation);
	virtual sf::Rect<float> GetBoundingRect() const;
	virtual void AddPositionToCache(double time, sf::Vector2f newPosition);
	virtual std::list<PositionStamp> ReturnPositionCache();
	void InterpolateMovement();
	static sf::Vector2f ReturnVelocity(PositionStamp oldPosition, PositionStamp newPosition);
protected:
	sf::Sprite& GetSprite();
	std::list<PositionStamp> positionsCache;
private:
	sf::Sprite  _sprite;
	sf::Texture _image;
	std::string _filename;
	bool _isLoaded;


};

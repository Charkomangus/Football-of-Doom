/*********************************************************************************
* Player class - inherits from sprite object - manages input/movement and rotation
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/


#pragma once
#include "SpriteObject.h"
class Player :
	public SpriteObject
{
public:
	Player();
	~Player();

	void Update(float elapsedTime) override;
	void ReceiveInput(sf::Vector2f _velocity) override;
	void UpdateRotation();
	sf::Vector2f UpdateInput();
	void Draw(sf::RenderWindow& rw) override;
	void ScreenBounds();
	sf::Vector2<float> GetVelocity() const;
	void SetSprite(std::string filename);
	bool update;
private:
	sf::Vector2<float> _velocity;
	float _maxVelocity, speed;	
	
	
};


#pragma once
#include "SpriteObject.h"
class Player :
	public SpriteObject
{
public:
	Player();
	~Player();

	void Update(float elapsedTime) override;
	void UpdateRotation();
	void Draw(sf::RenderWindow& rw) override;
	void SetPlayerNumber(int number);
	sf::Vector2<float> GetVelocity() const;

private:
	sf::Vector2<float> _velocity;
	float _maxVelocity;
	int playerNumber;
	
};


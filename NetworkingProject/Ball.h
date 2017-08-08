/*********************************************************************************
* Ball class - Updates it's movement and oversees it's collisions
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#pragma once
#include "SpriteObject.h"

class Player;

class Ball :
	public SpriteObject
{
public:
	Ball();
	virtual ~Ball();
	void Update(float) override;	

private:
	void Collision(Player* player);
	void Kicked(Player* player);
	float moveByX;
	float moveByY;
	float _velocity;
	float _maxVelocity;
	float _angle;
	float _elapsedTimeSinceStart;	
	float LinearVelocityX(float angle) const;
	float LinearVelocityY(float angle) const;
};

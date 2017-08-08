/*********************************************************************************
* Ball class - Updates it's movement and oversees it's collisions
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#include "StdAfx.h"
#include "Ball.h"


Ball::Ball() :	_velocity(0),	_maxVelocity(600),	_elapsedTimeSinceStart(0.0f)
{
	SpriteObject::Load("images/ball.png");
	assert(IsLoaded());
	
	float random_integer = std::rand() % 360; //Gives random number between 1 and 360.
	_angle = random_integer;
}


Ball::~Ball()
{
}

//Parameter is the time since last frame in seconds. VERY small number.
void Ball::Update(float elapsedTime) 
{
	_elapsedTimeSinceStart += elapsedTime;
	float moveAmount = _velocity  * elapsedTime;
	moveByX = LinearVelocityX(_angle) * moveAmount;
	moveByY = LinearVelocityY(_angle) * moveAmount;

	//Bounce off the walls - this is in addition to its inate screen bounding via spriteObject
	if (GetPosition().x + moveByX <= 0 + GetWidth() / 2 || GetPosition().x + GetWidth() / 2 + moveByX >= GameManager::SCREEN_WIDTH)
	{
		//Ricochet!
		_angle = 360.0f - _angle;
		if (_angle > 260.0f && _angle < 280.0f) _angle += 20.0f;
		if (_angle > 80.0f && _angle < 100.0f) _angle += 20.0f;
		moveByX = -moveByX;
	}
	//Bounce off the walls - this is in addition to its inate screen bounding via spriteObject
	if (GetPosition().y - GetHeight() / 2 <= 0 || GetPosition().y + GetHeight() / 2 >= GameManager::SCREEN_HEIGHT)
	{
		//Ricochet!
		_angle = 180 - _angle;
		if (_angle > 260.0f && _angle < 280.0f) _angle += 20.0f;
		if (_angle > 80.0f && _angle < 100.0f) _angle += 20.0f;
		moveByY = -moveByY;
	}
	GetSprite().move(moveByX, moveByY);


	//Collision
	Player* player1 = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player1"));
	Player* player2 = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player2"));
	Collision(player1);
	Collision(player2);


	if (_velocity > _maxVelocity)
		_velocity = _maxVelocity;
	else if(_velocity < -_maxVelocity)
		_velocity = -_maxVelocity;

	if (_velocity > 0)
		_velocity -= 1;
	if (_velocity < 0)
		_velocity += 1;


}


//If the ball collides with a player it gets kicked!
void Ball::Collision(Player* player)
{
	if (player == nullptr) return;

	sf::Rect<float> p1BB = player->GetBoundingRect();
	if (p1BB.intersects(GetBoundingRect()))
	{	
		Kicked(player);
	}
}

//Fling self in an angle 
void Ball::Kicked(Player* player)
{
	float _pi = 3.1428;
	_angle = -atan2(player->GetPosition().y - GetPosition().y, player->GetPosition().x - GetPosition().x) * (180 / _pi);
	_velocity += 10;
	
}


float Ball::LinearVelocityX(float angle) const
{
	angle -= 90;
	if (angle < 0) angle = 360 + angle;
	return (float)std::cos(angle * (3.1415926 / 180.0f));
}

float Ball::LinearVelocityY(float angle) const
{
	angle -= 90;
	if (angle < 0) angle = 360 + angle;
	return (float)std::sin(angle * (3.1415926 / 180.0f));
}

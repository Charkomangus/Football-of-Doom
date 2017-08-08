/*********************************************************************************
* Player class - inherits from sprite object - manages input/movement and rotation
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/

#include "stdafx.h"
#include "Player.h"
#include "GameManager.h"
#include "Ball.h"
#include <chrono>


Player::Player() :	_velocity(0,0), _maxVelocity(300), speed(25)
{
	SpriteObject::Load("images/Player1.png");
	assert(IsLoaded());

	GetSprite().setOrigin(GetSprite().getLocalBounds().width / 2, GetSprite().getLocalBounds().height / 2);
	update = true;
}

Player::~Player()
{
}


void Player::Draw(sf::RenderWindow & rw)
{
	SpriteObject::Draw(rw);
}



//Ensure the player stays in the screen
void Player::ScreenBounds()
{
	sf::Vector2f pos = this->GetPosition();
	if (pos.x  < GetSprite().getLocalBounds().width / 2)
	{
		SetPosition(GetSprite().getLocalBounds().width / 2, GetPosition().y);
		_velocity.x = -_velocity.x / 2;// Bounce by current velocity in opposite direction
	}
	if (pos.x >GameManager::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2)
	{
		SetPosition(GameManager::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2, GetPosition().y);
		_velocity.x = -_velocity.x / 2;// Bounce by current velocity in opposite direction
	}
	if (pos.y  < GetSprite().getLocalBounds().height / 2)
	{
		SetPosition(GetPosition().x, GetSprite().getLocalBounds().height / 2);
		_velocity.y = -_velocity.y / 2; // Bounce by current velocity in opposite direction
	}
	if (pos.y >GameManager::SCREEN_HEIGHT - GetSprite().getLocalBounds().height / 2)
	{
		SetPosition(GetPosition().x, GameManager::SCREEN_HEIGHT - GetSprite().getLocalBounds().height / 2);
		_velocity.y = -_velocity.y / 2; // Bounce by current velocity in opposite direction
	}
}

sf::Vector2<float> Player::GetVelocity() const
{
	return _velocity;
}

//Change image
void Player::SetSprite(std::string filename)
{
	SpriteObject::Load(filename);
	assert(IsLoaded());
}


void Player::Update(float elapsedTime)
{
	
	//Ensure the players dont exit the screen
	ScreenBounds();

	//Rotation is dependant on the ball. As this will moving via interpolated movement we can just set the player to look at it.
	UpdateRotation();

	if (!update) return;
	//This player is controlled via the server TEMP
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		_velocity.y -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_velocity.y += speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		_velocity.x -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_velocity.x += speed;
	}
	_velocity.x = std::min(_maxVelocity, std::max(_velocity.x, -_maxVelocity));
	_velocity.y = std::min(_maxVelocity, std::max(_velocity.y, -_maxVelocity));


	if (_velocity.x > 0)
		_velocity.x -= speed / 2;
	if (_velocity.x < 0)
		_velocity.x += speed / 2;

	if (_velocity.y < 0)
		_velocity.y += speed / 2;
	if (_velocity.y > 0)
		_velocity.y -= speed / 2;


	//COLLISION 
	sf::Rect<float> p1BB = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player2"))->GetBoundingRect();
	
	if (p1BB.intersects(GetBoundingRect()))
	{
		_velocity.x = -_velocity.x / 2;
		_velocity.y = -_velocity.y / 2;
		SetPosition(GetPosition().x + _velocity.x / 10, GetPosition().y + _velocity.y / 10);
	}

	//MOVE
	GetSprite().move(_velocity * elapsedTime);

}

//Return frame input
sf::Vector2f Player::UpdateInput()
{
	//Ensure the players dont exit the screen
	ScreenBounds();

	//Rotation is dependant on the ball. As this will moving via interpolated movement we can just set the player to look at it.
	UpdateRotation();


	sf::Vector2f input;
	//This player is controlled via the server TEMP
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		input.y -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		input.y += speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		input.x -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		input.x += speed;
	}	
	return input;
}


void Player::UpdateRotation()
{	
	Ball* ball = dynamic_cast<Ball*>(GameManager::GetGameObjectManager().Get("Ball"));
	const float PI = 3.14159265;
	float dx = GetPosition().x - ball->GetPosition().x;
	float dy = GetPosition().y - ball->GetPosition().y;
	float rotation = (atan2(dy, dx)) * 180 / PI;
	GetSprite().setRotation(rotation);
}

//Mpve using given input
void Player::ReceiveInput(sf::Vector2f position)
{
	//_velocity -= GetPosition();
	_velocity += position;
	//Ensure the players dont exit the screen
	ScreenBounds();

	//Rotation is dependant on the ball. As this will moving via interpolated movement we can just set the player to look at it.
	UpdateRotation();
	
	_velocity.x = std::min(_maxVelocity, std::max(_velocity.x, -_maxVelocity));
	_velocity.y = std::min(_maxVelocity, std::max(_velocity.y, -_maxVelocity));


	if (_velocity.x > 0)
		_velocity.x -= speed / 2;
	if (_velocity.x < 0)
		_velocity.x += speed / 2;

	if (_velocity.y < 0)
		_velocity.y += speed / 2;
	if (_velocity.y > 0)
		_velocity.y -= speed / 2;


	//COLLISION 
	sf::Rect<float> p1BB = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player1"))->GetBoundingRect();

	if (p1BB.intersects(GetBoundingRect()))
	{
		_velocity.x = -_velocity.x / 2;
		_velocity.y = -_velocity.y / 2;
		SetPosition(GetPosition().x + _velocity.x / 10, GetPosition().y + _velocity.y / 10);
	}
	
	//MOVE
	std::cout << _velocity.x << " " << _velocity.y << std::endl;
	

	GetSprite().move(_velocity / 10.0f);
}

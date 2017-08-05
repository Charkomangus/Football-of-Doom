//Player class

#include "stdafx.h"
#include "Player.h"
#include "GameManager.h"
#include "Ball.h"


Player::Player() :	_velocity(0,0), _maxVelocity(500), playerNumber(1)
{
	SpriteObject::Load("images/paddle.png");
	assert(IsLoaded());

	GetSprite().setOrigin(GetSprite().getLocalBounds().width / 2, GetSprite().getLocalBounds().height / 2);

}

Player::~Player()
{
}


void Player::Draw(sf::RenderWindow & rw)
{
	SpriteObject::Draw(rw);
}

void Player::SetPlayerNumber(int number)
{
	playerNumber = number;
}

sf::Vector2<float> Player::GetVelocity() const
{
	return _velocity;
}

void Player::Update(float elapsedTime)
{	
	if (playerNumber != 1) return;		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			_velocity.y -= 0.5f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			_velocity.y += 0.5f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			_velocity.x -= 0.5f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			_velocity.x += 0.5f;
		}
		_velocity.x = std::min(_maxVelocity, std::max(_velocity.x, -_maxVelocity));
		_velocity.y = std::min(_maxVelocity, std::max(_velocity.y, -_maxVelocity));

		
		if (_velocity.x > 0)
			_velocity.x -= 0.3;
		if (_velocity.x < 0)
			_velocity.x += 0.3;

		if (_velocity.y < 0)
			_velocity.y+= 0.3;
		if (_velocity.y > 0)
			_velocity.y-= 0.3;
	
	

	sf::Vector2f pos = this->GetPosition();

	if (pos.x  < GetSprite().getLocalBounds().width / 2	|| pos.x >(GameManager::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2))
	{
		_velocity.x = -_velocity.x; // Bounce by current velocity in opposite direction
	}
	if (pos.y  < GetSprite().getLocalBounds().height / 2 || pos.y >(GameManager::SCREEN_HEIGHT - GetSprite().getLocalBounds().height / 2))
	{
		_velocity.y = -_velocity.y; // Bounce by current velocity in opposite direction
	}

	GetSprite().move(_velocity * elapsedTime);

	
	UpdateRotation();
	
	

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

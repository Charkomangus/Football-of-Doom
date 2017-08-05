//Player class

#include "stdafx.h"
#include "Player.h"
#include "GameManager.h"
#include "Ball.h"


Player::Player() :	_velocity(0,0), _maxVelocity(600), playerNumber(1), speed(10)
{
	SpriteObject::Load("images/Player1.png");
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
	if(number == 1)
		SpriteObject::Load("images/Player1.png");
	else 
		SpriteObject::Load("images/Player2.png");
	playerNumber = number;
}

sf::Vector2<float> Player::GetVelocity() const
{
	return _velocity;
}

void Player::Update(float elapsedTime)
{
	
	//Collision
	if (playerNumber == 1) 
	{
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
			_velocity.x -= 1;
		if (_velocity.x < 0)
			_velocity.x += 1;

		if (_velocity.y < 0)
			_velocity.y += 1;
		if (_velocity.y > 0)
			_velocity.y -= 1;



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

		sf::Rect<float> p1BB = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player2"))->GetBoundingRect();

		if (p1BB.intersects(GetBoundingRect()))
		{
			_velocity.x = -_velocity.x / 2;
			_velocity.y = -_velocity.y / 2;
			SetPosition(GetPosition().x + _velocity.x/2, GetPosition().y + _velocity.y/2);
		}

		GetSprite().move(_velocity * elapsedTime);
	
	

	}
	else if(playerNumber == 2)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			_velocity.y -= speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			_velocity.y += speed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			_velocity.x -= speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			_velocity.x += speed;
		}
		_velocity.x = std::min(_maxVelocity, std::max(_velocity.x, -_maxVelocity));
		_velocity.y = std::min(_maxVelocity, std::max(_velocity.y, -_maxVelocity));

		if (_velocity.x > 0)
			_velocity.x -= 1;
		if (_velocity.x < 0)
			_velocity.x += 1;

		if (_velocity.y < 0)
			_velocity.y += 1;
		if (_velocity.y > 0)
			_velocity.y -= 1;


		sf::Vector2f pos = this->GetPosition();

		if (pos.x  < GetSprite().getLocalBounds().width / 2)
		{
			SetPosition(GetSprite().getLocalBounds().width / 2, GetPosition().y);
			_velocity.x = -_velocity.x / 2;// Bounce by current velocity in opposite direction
		}
		if(pos.x >GameManager::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2)
		{
			SetPosition(GameManager::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2, GetPosition().y);
			_velocity.x = -_velocity.x / 2;// Bounce by current velocity in opposite direction
		}
		if (pos.y  < GetSprite().getLocalBounds().height / 2)
		{
			SetPosition(GetPosition().x, GetSprite().getLocalBounds().height / 2);
			_velocity.y = -_velocity.y / 2; // Bounce by current velocity in opposite direction
		}
		if(pos.y >GameManager::SCREEN_HEIGHT - GetSprite().getLocalBounds().height / 2)
		{
			SetPosition(GetPosition().x, GameManager::SCREEN_HEIGHT - GetSprite().getLocalBounds().height / 2);
			_velocity.y = -_velocity.y / 2; // Bounce by current velocity in opposite direction
		}
				
		sf::Rect<float> p1BB = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player1"))->GetBoundingRect();

		if (p1BB.intersects(GetBoundingRect()))
		{
			_velocity.x = -_velocity.x/2;
			_velocity.y = -_velocity.y/2;
			SetPosition(GetPosition().x + _velocity.x / 2, GetPosition().y + _velocity.y / 2);
		}
		GetSprite().move(_velocity * elapsedTime);


	}	
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

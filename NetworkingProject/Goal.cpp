/*********************************************************************************
* Goal class - Checks if its been collided with
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/

#include "stdafx.h"
#include "Goal.h"


Goal::Goal(): goalNumber(0)
{
	SpriteObject::Load("images/goal.png");
	assert(IsLoaded());	
}


Goal::~Goal()
{
}

//Collision
void Goal::Update(float)
{
	Ball* ball = dynamic_cast<Ball*>(GameManager::GetGameObjectManager().Get("Ball"));	
	Collision(ball);	
}

void Goal::SetGoalNumber(int newNumber)
{
	goalNumber = newNumber;
}

void Goal::Collision(Ball* ball) const
{
	if (ball == nullptr) return;

	sf::Rect<float> ballRect = ball->GetBoundingRect();
	if (ballRect.intersects(GetBoundingRect()))
	{
		std::cout << "GOAAAAALLLLL" << std::endl;
		std::cout << "Player "  << goalNumber << " wins!" << std::endl;
		GameManager::SetState(0);
		
	}
}

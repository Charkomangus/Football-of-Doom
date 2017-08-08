/*********************************************************************************
* Goal class - Checks if its been collided with
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#pragma once
class Goal :
	public SpriteObject
{
public:
	Goal();
	virtual ~Goal();
	void Update(float) override;
	void SetGoalNumber(int newNumber);
private:
	void Collision(Ball* ball) const;	
	int goalNumber;
};


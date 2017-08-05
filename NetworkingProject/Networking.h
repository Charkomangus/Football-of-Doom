#pragma once
#include "Ball.h"


class Networking 
{
public:	
	Networking();
	~Networking();
	void UpdatePosition(std::string id, SpriteObject* player);
	void ReceivePosition(std::string id, SpriteObject* player);
	void SetAsClient();
	void SetAsServer();
private:
	sf::TcpListener listener;
	sf::IpAddress ip;
	sf::TcpSocket socket;
	Player* player1;
	Player* player2;
	Ball* ball;
	sf::Vector2f prevPosition;
	bool hasBeenSet;
	sf::Vector2f player1PreviousPosition, player2PreviousPosition, ballPreviousPosition;

	
};


#pragma once
#include "Ball.h"
class Player;

class Networking 
{
public:	
	Networking();
	~Networking();
	void UpdatePreviousPosition();
	void SendPositionPacket(sf::Vector2f position, sf::Vector2f prevPosition);
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
	
};


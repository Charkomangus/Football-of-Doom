/*********************************************************************************
* Networking class - Can set instance as client or server - Can send and receive packages
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#pragma once
#include "Ball.h"

struct ObjectPacket
{
	std::string id;
	float x, y, rotation;
};

class Networking 
{
public:	
	Networking();
	~Networking();
	void UpdatePosition(std::string id, SpriteObject* player);
	void ReceivePosition(std::string id, SpriteObject* player);
	void SetAsClient();
	void SetAsServer();
	void SendObjectPacket(ObjectPacket objectPacket);
	void SendWinMessage();
	void SendKillMessage();
	void SendInput(std::string id, sf::Vector2f _velocity);
	bool hasBeenSet;
private:	
	sf::TcpSocket socket;
	sf::TcpListener listener;
	sf::IpAddress ip;
	Player* player1;
	Player* player2;
	Ball* ball;
	sf::Vector2f prevPosition;
	
	sf::Vector2f player1PreviousPosition, player2PreviousPosition, ballPreviousPosition;
	sf::Clock clock;
	int timeOut;
	int timesTried;
};


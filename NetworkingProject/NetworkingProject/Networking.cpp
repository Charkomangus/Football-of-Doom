#include "stdafx.h"
#include "Networking.h"
#include "GameManager.h"
#include "Ball.h"

struct ObjectPacket
{
	std::string id;
	float x, y;
};

Networking::Networking(): hasBeenSet(false)
{	
	ip = sf::IpAddress::getLocalAddress();
	player1 = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player1"));
	player2 = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player2"));
	ball = dynamic_cast<Ball*>(GameManager::GetGameObjectManager().Get("Ball"));
	socket.setBlocking(false);
}

Networking::~Networking()
{
}

void Networking::SetAsServer()
{
	
	if (hasBeenSet) return;
	hasBeenSet = true;

	// ----- The server -----
	std::cout << "SERVER INITIALISED" << std::endl;
	// Create a listener to wait for incoming connections on port 2000
	sf::TcpListener listener;	
	if (listener.listen(5000) != sf::Socket::Done)
	{
		hasBeenSet = false;
		std::cout << "Listener cannot connect" << std::endl;
		return;
	}

	//Wait for a connection	
	if (listener.accept(socket) != sf::Socket::Done)
	{
		hasBeenSet = false;
		std::cout << "Listener cannot accept connection" << std::endl;
		return;
	}

	std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;	
	
		
}

void Networking::SetAsClient()
{
	if (hasBeenSet) return;
	
	ip = sf::IpAddress::getLocalAddress();
	// ----- The client -----
	std::cout << "CLIENT INITIALISED" << std::endl;
	sf::Socket::Status status = socket.connect(ip, 5000);	
	if (status == sf::Socket::Done)
	{
		hasBeenSet = true;
		std::cout << "Client connected to Server" << std::endl;
	}
	else 
	{
		hasBeenSet = false;
		std::cout << "Client cannot find Server - RESTARTING" << std::endl;	
	}
	
}


//Set the previous position of the player
void Networking::UpdatePreviousPosition()
{
	if (player1 == nullptr)
	{
		player1 = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player1"));
		std::cout << "player was null" << std::endl;
	}
	prevPosition = player1->GetPosition();
}



void Networking::UpdatePosition(std::string id, SpriteObject* object)
{
	ObjectPacket objectPacket;
	sf::Packet packet;
	int x = object->GetPosition().x;
	int y = object->GetPosition().y;
	objectPacket.id = id;
	objectPacket.x = x;
	objectPacket.y = y;

	packet << objectPacket.id << objectPacket.x << objectPacket.y;
	socket.send(packet);	
	packet.clear();
}

void Networking::ReceivePosition(std::string id, SpriteObject* object)
{
	ObjectPacket objectPacket;
	sf::Packet packet;
	socket.receive(packet);

	if (packet >> objectPacket.id >> objectPacket.x >> objectPacket.y)
	{
		if (id == objectPacket.id)
		{
			if (object->GetPosition().x != objectPacket.x || object->GetPosition().y != objectPacket.y)
			{
				object->SetPosition(objectPacket.x, objectPacket.y);
				
			}

		}
	}	
}
	
	


sf::Packet& operator <<(sf::Packet& packet, const ObjectPacket& p)
{
	return packet << p.id << p.x << p.y;
}
sf::Packet& operator >> (sf::Packet& packet, ObjectPacket& p)
{
	return packet >> p.id >> p.x >> p.y;
}



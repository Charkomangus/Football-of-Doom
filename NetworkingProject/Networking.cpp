#include "stdafx.h"
#include "Networking.h"
#include "GameManager.h"
#include "Ball.h"

struct ObjectPacket
{
	std::string id;
	float x, y, rotation;
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




void Networking::UpdatePosition(std::string id, SpriteObject* object)
{
	ObjectPacket objectPacket;
	sf::Packet packet;
	objectPacket.id = id;
	objectPacket.x = object->GetPosition().x;
	objectPacket.y = object->GetPosition().y;
	objectPacket.rotation = object->GetSprite().getRotation();

	
	if (id == "Player1")
	{
		if (objectPacket.x != player1PreviousPosition.x || objectPacket.y != player1PreviousPosition.y)
		{
			std::cout << " SENT PACKET: " << objectPacket.id << " " << object->GetPreviousPosition().x << " and " << objectPacket.x << std::endl;
			packet << objectPacket.id << objectPacket.x << objectPacket.y << objectPacket.rotation;
			socket.send(packet);
			packet.clear();
			player1PreviousPosition = object->GetPosition();
		}	
	}
	else if (id == "Player2")
	{
		if (objectPacket.x != player2PreviousPosition.x || objectPacket.y != player2PreviousPosition.y)
		{
			std::cout << " SENT PACKET: " << objectPacket.id << " " << object->GetPreviousPosition().x << " and " << objectPacket.x << std::endl;
			packet << objectPacket.id << objectPacket.x << objectPacket.y << objectPacket.rotation;
			socket.send(packet);
			packet.clear();
			player2PreviousPosition = object->GetPosition();
		}
	}
	else if (id == "Ball")
	{
		if (objectPacket.x != ballPreviousPosition.x || objectPacket.y != ballPreviousPosition.y)
		{
			std::cout << " SENT PACKET: " << objectPacket.id << " " << object->GetPreviousPosition().x << " and " << objectPacket.x << std::endl;
			packet << objectPacket.id << objectPacket.x << objectPacket.y << objectPacket.rotation;
			socket.send(packet);
			packet.clear();
			ballPreviousPosition = object->GetPosition();
		}
	}	
	
}

void Networking::ReceivePosition(std::string id, SpriteObject* object)
{
	ObjectPacket objectPacket;
	sf::Packet packet;
	socket.receive(packet);

	if (packet >> objectPacket.id >> objectPacket.x >> objectPacket.y >> objectPacket.rotation)
	{				
		if (id == objectPacket.id)
		{
			if (object->GetPosition().x != objectPacket.x || object->GetPosition().y != objectPacket.y || objectPacket.rotation != object->GetSprite().getRotation())
			{
				object->SetPosition(objectPacket.x, objectPacket.y);
				object->GetSprite().setRotation(objectPacket.rotation);
				
			}

		}
	}	
}


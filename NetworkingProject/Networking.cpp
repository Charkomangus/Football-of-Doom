/*********************************************************************************
* Networking class - Can set instance as client or server - Can send and receive packages
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#include "stdafx.h"
#include "Networking.h"
#include "GameManager.h"
#include "Ball.h"

Networking::Networking(): hasBeenSet(false), timeOut(300), timesTried(0)
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

//Server logic - this will run once per frame
void Networking::SetAsServer()
{
	//If the server has tried to listen unsuccesfully too many times then close the program
	if (timesTried > timeOut)
	{
		GameManager::ExitGame();
		return;
	}
	if (hasBeenSet) return;
	

	//Setup section
	if (!hasBeenSet)
	{		
		//Wait for a connection	
		if (listener.listen(5000) != sf::Socket::Done)
		{
			std::cout << "Listener failed" << std::endl;
			hasBeenSet = false;
			timesTried++;
			return;
		}
		std::cout << "SERVER INITIALISED" << std::endl;
	}
	hasBeenSet = true;
	//Wait for a connection	
	if (listener.accept(socket) == sf::Socket::Done)
	{
		std::cout << "Listener accepted connection" << std::endl;
		std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
		
	}
	
	//Reset timeout 
	timesTried = 0;


}

//Client logic - this will run once per frame
void Networking::SetAsClient()
{
	//If the client has tried to connect too many times close the program
	if (timesTried > timeOut)
	{
		GameManager::ExitGame();
		return;
	}	
	sf::Socket::Status status = {};
	//Setup section
	if (!hasBeenSet) 
	{
		hasBeenSet = true;			
		status = socket.connect(ip, 5000);
	

		//Connection confirmation - reset timeout
		if (status == sf::Socket::Done)
		{
			std::cout << "CLIENT INITIALISED" << std::endl;
			std::cout << "Client connected to Server" << std::endl;
			timesTried = 0;
		}

	}
	

	//Error Handling - try until success or timeout
	if (status == sf::Socket::Error)
	{		
		hasBeenSet = false;
		std::cout << "Client Error - RESTARTING " << timesTried << "/" << timeOut << std::endl;
		socket.connect(ip, 5000);
		timesTried++;
	}
	else if (status == sf::Socket::Disconnected)
	{
		hasBeenSet = false;
		std::cout << "Client Error - Disconnected " << timesTried << "/" << timeOut <<  std::endl;
		socket.connect(ip, 5000);
		timesTried++;
	}
	else if (status == sf::Socket::NotReady)
	{		
		hasBeenSet = false;
		std::cout << "Client Error - Not Ready " << timesTried << "/" << timeOut << std::endl;
		socket.connect(ip, 5000);
		timesTried++;
	}	
}

//If movement has been made by an object create a packet with its id, position and rotation and send it
void Networking::UpdatePosition(std::string id, SpriteObject* object)
{
	//Create a object packet using the given objects traits
	ObjectPacket objectPacket;
	sf::Packet packet;
	objectPacket.id = id;
	objectPacket.x = object->GetPosition().x;
	objectPacket.y = object->GetPosition().y;
	objectPacket.rotation = object->GetRotation();
		
	sf::Vector2f previousPosition;
	//Determine which object needs to be updated
	if (id == "Player1")	
		previousPosition = player1PreviousPosition;
	if (id == "Player2")	
		previousPosition = player2PreviousPosition;		
	if (id == "Ball")
		previousPosition = ballPreviousPosition;
	
	//If the object has moved since the last packet send a packet
	if (objectPacket.x != previousPosition.x || objectPacket.y != previousPosition.y)
	{
		//std::cout << " SENT PACKET: " << objectPacket.id << " " << previousPosition.x << " / " << objectPacket.x << " and " << previousPosition.y << " / " << objectPacket.y << std::endl;
		SendObjectPacket(objectPacket);	
		//Update the objects previous position
		if (id == "Player1")
			player1PreviousPosition = object->GetPosition();
		if (id == "Player2")
			player2PreviousPosition = object->GetPosition();
		if (id == "Ball")
			ballPreviousPosition = object->GetPosition();
	}
	packet.clear();
}

//If movement has been made by an object create a packet with its id, position and rotation and send it
void Networking::SendInput(std::string id, sf::Vector2f _velocity)
{
	//Create a object packet using the given objects traits
	ObjectPacket objectPacket;
	sf::Packet packet;
	objectPacket.id = id+"Input";
	objectPacket.x = _velocity.x;
	objectPacket.y = _velocity.y;
	objectPacket.rotation = 0;
	SendObjectPacket(objectPacket);

}

//Receive a position packet and save it in the positional cache of the relevant object
void Networking::ReceivePosition(std::string id, SpriteObject* object)
{
	ObjectPacket objectPacket;
	sf::Packet packet;
	socket.receive(packet);

	if (packet >> objectPacket.id >> objectPacket.x >> objectPacket.y >> objectPacket.rotation)
	{
		std::cout << objectPacket.id << std::endl;
		//Win program
		if (id == "WIN")
		{
			std::cout << "HOMAMA" << std::endl;
			GameManager::SetState(0);
			return;
		}
		//Kill program
		if (id == "KILL")
		{
			std::cout << "DISCONNECTED" << std::endl;
			hasBeenSet = false;
			socket.disconnect();
			return;
		}
		//Receive input commands
		if (id == "Player2Input")
		{			
			sf::Vector2f temp;
			temp.x = objectPacket.x;
			temp.y = objectPacket.y;			
			object->ReceiveInput(temp);
			return;
		}
		if (id == objectPacket.id)
		{			
			//When the difference between positions is too large to be normal teleport the object to the correct position
			//This is a bit of a cheat but it does help correct any errors that crop up
			if (objectPacket.x - object->GetPosition().x > 1 || objectPacket.y - object->GetPosition().y > 1)
			{				
				object->SetPosition(objectPacket.x, objectPacket.y);
			}
			//Add position to be used in interpolation
			else
			{				
				double temp = clock.getElapsedTime().asSeconds();
				object->AddPositionToCache(temp, sf::Vector2f(objectPacket.x, objectPacket.y));
			}		
			//Set items rotation
			object->SetRotation(objectPacket.rotation);
		}
	}	
	packet.clear();
}

//Send an object Package
void Networking::SendObjectPacket(ObjectPacket objectPacket)
{
	sf::Packet packet;
	packet << objectPacket.id << objectPacket.x << objectPacket.y << objectPacket.rotation;
	socket.send(packet);
	packet.clear();
}

//Send a message that tells the other side that victory happened
void Networking::SendWinMessage()
{
	sf::Packet packet;
	packet << "WIN" << 1 << 1 << 1;
	socket.send(packet);
	packet.clear();
}

//Send a message that tells the other side that victory happened
void Networking::SendKillMessage()
{
	std::cout << "DISCONNECTED" << std::endl;
	sf::Packet packet;
	packet << "KILL" << 1 << 1 << 1;
	socket.send(packet);
	packet.clear();
	socket.disconnect();
}

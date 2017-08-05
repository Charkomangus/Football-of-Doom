//Central project class - manages gameplay

#include "stdafx.h"
#include "GameManager.h"
#include "SplashScreen.h"
#include "Menu.h"
#include "Ball.h"
#include "BackGround.h"


void GameManager::Start(void)
{
	if (_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "Bonk's Football of Doom!");
	_mainWindow.setFramerateLimit(30);

	//Setting Game background
	BackGround *background = new BackGround();
	
	
	Networking *_networking = new Networking();
	
	//Adding a player
	Player *player1 = new Player();
	player1->SetPosition(100, SCREEN_HEIGHT / 2);
	player1->SetPlayerNumber(1);
	Player *player2 = new Player();
	player2->SetPosition(SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2);
	player2->SetPlayerNumber(3);
	////Adding a ball
	Ball *ball = new Ball();
	ball->SetPosition((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 15);

	_gameObjectManager.Add("Background", background);
	_gameObjectManager.Add("Player1", player1);
	_gameObjectManager.Add("Player2", player2);
	_gameObjectManager.Add("Ball", ball);

	_gameState = ShowingSplash;	
	
	while (!IsExiting())
	{
			GameLoop(_networking);
		
	}

	_mainWindow.close();

}

bool GameManager::IsExiting()
{
	if (_gameState == Exiting)
		return true;
	return false;
}

sf::RenderWindow& GameManager::GetWindow()
{
	return _mainWindow;
}

const sf::Event& GameManager::GetInput()
{
	sf::Event currentEvent;
	_mainWindow.pollEvent(currentEvent);
	return currentEvent;
}


void GameManager::GameLoop(Networking* _networking)
{
	
	switch (_gameState)
	{
		case ShowingMenu:
		{
		
			ShowMenu();
			break;
		}
		case ShowingSplash:
		{
			ShowSplashScreen();
			break;
		}
		case PlayingServer:		
		{
			_mainWindow.clear(sf::Color(0, 0, 0));
			_networking->SetAsServer();
			_gameObjectManager.UpdateAll();
			_networking->ReceivePosition("Player2", _gameObjectManager.Get("Player2"));					
			_networking->UpdatePosition("Player1", _gameObjectManager.Get("Player1"));
			_networking->UpdatePosition("Ball", _gameObjectManager.Get("Ball"));			
			_gameObjectManager.DrawAll(_mainWindow);
			_mainWindow.display();
			
			sf::Event currentEvent;
			_mainWindow.pollEvent(currentEvent);
			_gameObjectManager.SetPreviousPositionAll();
			if (currentEvent.type == sf::Event::Closed)
				_gameState = Exiting;
			if (currentEvent.type == sf::Event::KeyPressed)
			{
				if (currentEvent.key.code == sf::Keyboard::Escape) ShowMenu();
			}
			break;
		}
		case PlayingClient:
		{			
			Player* player2 = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player2"));
			_mainWindow.clear(sf::Color(0, 0, 0));			
			_networking->SetAsClient();		
			_networking->ReceivePosition("Player1", _gameObjectManager.Get("Player1"));			
			_networking->ReceivePosition("Ball", _gameObjectManager.Get("Ball"));	
			

			player2->SetPlayerNumber(2);
			_gameObjectManager.UpdateObject("Player2");
			_networking->UpdatePosition("Player2", player2);
		
			_gameObjectManager.DrawAll(_mainWindow);
			_mainWindow.display();

			sf::Event currentEvent;
			_mainWindow.pollEvent(currentEvent);
			if (currentEvent.type == sf::Event::Closed)
				_gameState = Exiting;
			if (currentEvent.type == sf::Event::KeyPressed)
			{
				if (currentEvent.key.code == sf::Keyboard::Escape) ShowMenu();
			}
				
			break;
		}
	default: ;
	}
	
}


void GameManager::ShowSplashScreen()
{
	SplashScreen splashScreen;
	splashScreen.Show(_mainWindow);
	_gameState = ShowingMenu;
}

void GameManager::ShowMenu()
{
	Menu mainMenu;
	Menu::MenuResult result = mainMenu.Show(_mainWindow);
	switch (result)
	{
	case Menu::Exit:
		_gameState = Exiting;
		break;
	case Menu::PlayServer:
		_gameState = PlayingServer;	 
		break;
	case Menu::PlayClient:	
		_gameState = PlayingClient;	
		break;
	default: ;
	}
}



const GameObjectManager& GameManager::GetGameObjectManager()
{
	return GameManager::_gameObjectManager;
}

GameManager::GameState GameManager::_gameState = Uninitialized;
sf::RenderWindow GameManager::_mainWindow;
GameObjectManager GameManager::_gameObjectManager;
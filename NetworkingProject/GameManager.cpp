/*********************************************************************************
* Central project class - manages gameplay, networking, everything really
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#include "stdafx.h"
#include "GameManager.h"
#include "SplashScreen.h"
#include "Menu.h"
#include "Ball.h"
#include "BackGround.h"
#include "Goal.h"
#include "Win.h"

void GameManager::Start(void)
{
	if (_gameState != Uninitialized)
		return;

	int framerate = 60;
	//Time since last update	
	float dt = 0.0f; 
	//Game updates once per frame
	const float TIME_STEP = 1.f/ framerate;
	
	sf::Clock clock;

	//Create a window and set the framerate to a limit
	_mainWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "Bonk's Football of Doom!");
	_mainWindow.setFramerateLimit(framerate);
	_mainWindow.setActive(true);
	//Setting up a networking instance
	Networking *_networking = new Networking();

	//Setting Game background
	BackGround *background = new BackGround("images/background.jpg");

	//Adding Goals
	Goal *goal1 = new Goal();
	goal1->SetPosition(25, SCREEN_HEIGHT / 2 - goal1->GetHeight() / 2);
	goal1->SetGoalNumber(2);
	Goal *goal2 = new Goal();
	goal2->SetPosition(SCREEN_WIDTH - 25, SCREEN_HEIGHT / 2 + goal2->GetHeight()/2);
	goal2->SetGoalNumber(1);
	goal2->SetRotation(180);

	//Adding players
	Player *player1 = new Player();
	player1->SetPosition(100, SCREEN_HEIGHT / 2);	

	Player *player2 = new Player();
	player2->SetPosition(SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2);
	player2->SetSprite("images/Player2.png");
	player2->update = false;
	////Adding a ball
	Ball *ball = new Ball();
	ball->SetPosition((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 15);

	//Adding all objects in a common pool
	_gameObjectManager.Add("Goal1", goal1);
	_gameObjectManager.Add("Goal2", goal2);
	_gameObjectManager.Add("Background", background);
	_gameObjectManager.Add("Player1", player1);
	_gameObjectManager.Add("Player2", player2);
	_gameObjectManager.Add("Ball", ball);

	//Beging game by showing the splash screen
	_gameState = ShowingSplash;	
	

	while (!IsExiting())	
	{
		
		//Clock is being updated;
		dt = clock.getElapsedTime().asSeconds(); 
	
		//Only happens once per frame FIXED TIMESTEP
		while (dt > TIME_STEP)
		{
			//std::cout << ">>>>>>>>>>> NEW UPDATE <<<<<<<<<<" << std::endl;
			GameUpdate(_networking);

			//Reset clock
			clock.restart();
			dt = 0;		
		}
		//Draw happens outside of fixed timestep
		GameDraw(_networking);
		
	}

	_networking->SendKillMessage();
	//Close window and exit application
	_mainWindow.close();
	exit(0);

}



void GameManager::GameUpdate(Networking* _networking)
{	
	switch (_gameState)
	{		
		case PlayingServer:		
		{	
			//Try and keep the server the focused window TEMP - Doesnt really work
			if (!_mainWindow.hasFocus())
				_mainWindow.requestFocus();

			//This is called until connection is made then just returns
			_networking->SetAsServer();	
			if (!_networking->hasBeenSet) return;
		
			//Update all game objects
			//Send the new positions of player1 to the client
			_gameObjectManager.UpdateAll();
			_networking->UpdatePosition("Player1", _gameObjectManager.Get("Player1"));

			//Send the new positions of ball to the client
			
			_networking->UpdatePosition("Ball", _gameObjectManager.Get("Ball"));			
			
			//Send the new positions of player2 to the client
			_networking->ReceivePosition("Player2Input", _gameObjectManager.Get("Player2"));
			_networking->UpdatePosition("Player2", _gameObjectManager.Get("Player2"));
		
		

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
		case PlayingClient:
		{
			//This is called until connection is made then just returns
			_networking->SetAsClient();		
			if (!_networking->hasBeenSet) return;
			//Receive player1's new position from server then interpolate its movement
			SpriteObject* player1 = _gameObjectManager.Get("Player1");
			_networking->ReceivePosition("Player1", player1);
			player1->InterpolateMovement();

			//Receive player1's new position from server then interpolate its movement
			//Set player 2 to be controlled by different keys - this is done simply to enable testing on the same machine.
			Player* player2 = dynamic_cast<Player*>(GameManager::GetGameObjectManager().Get("Player2"));

			if(player2->UpdateInput().x != 0 && player2->UpdateInput().y != 0)
				_networking->SendInput("Player2", player2->UpdateInput());
			_networking->ReceivePosition("Player2", player2);
			player2->InterpolateMovement();			
		

			//Receive ball's new position from server then interpolate its movement
			SpriteObject* ball = _gameObjectManager.Get("Ball");
			_networking->ReceivePosition("Ball", ball);
			ball->InterpolateMovement();		
						

			//Update goals
			_gameObjectManager.UpdateObject("Goal1");
			_gameObjectManager.UpdateObject("Goal2");

			

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


void GameManager::GameDraw(Networking* _networking)
{
	
	switch (_gameState)
	{	
		case ShowingMenu:
		{
			ShowMenu();
			break;
		}
		case Win:
		{
			ShowWin();
			_networking->SendWinMessage();
			break;
		}
		case ShowingSplash:
		{
			ShowSplashScreen("images/SplashScreen.png");
			break;
		}
		case PlayingServer:	
		case PlayingClient:
		{
			{
				//Do not proceed with the game if its been set
				if (!_networking->hasBeenSet) return;
				_mainWindow.clear(sf::Color(0, 0, 0));
				_gameObjectManager.DrawAll(_mainWindow);
				_mainWindow.display();			
			}
			break;
		}
		default:;
	}
}

//Show spalch screen and update it
void GameManager::ShowSplashScreen(std::string filename)
{
	SplashScreen splashScreen;
	splashScreen.Show(_mainWindow, filename);
	_gameState = ShowingMenu;
}

//Show menu and update it
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
	default:
		_gameState = ShowingMenu;
	}
}

//Show win screen and update it
void GameManager::ShowWin()
{
	::Win winscreen;
	winscreen.Show(_mainWindow);
	_gameState = Win;	
}

//Chaneg state
void GameManager::SetState(int state)
{
	if (state == 0)
		_gameState = Win;
}

//Restarts game
void GameManager::Restart()
{
	_gameObjectManager.RemoveAll();
	//Setting Game background
	BackGround *background = new BackGround("images/background.jpg");

	//Adding goals
	Goal *goal1 = new Goal();
	goal1->SetPosition(25, SCREEN_HEIGHT / 2 - goal1->GetHeight() / 2);
	goal1->SetGoalNumber(2);
	Goal *goal2 = new Goal();
	goal2->SetPosition(SCREEN_WIDTH - 25, SCREEN_HEIGHT / 2 + goal2->GetHeight() / 2);
	goal2->SetGoalNumber(1);
	goal2->SetRotation(180);

	//Adding players
	Player *player1 = new Player();
	player1->SetPosition(100, SCREEN_HEIGHT / 2);
	Player *player2 = new Player();
	player2->SetPosition(SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2);

	////Adding a ball
	Ball *ball = new Ball();
	ball->SetPosition((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 15);

	//Adding all objects in a common pool
	_gameObjectManager.Add("Goal1", goal1);
	_gameObjectManager.Add("Goal2", goal2);
	_gameObjectManager.Add("Background", background);
	_gameObjectManager.Add("Player1", player1);
	_gameObjectManager.Add("Player2", player2);
	_gameObjectManager.Add("Ball", ball);
}

//Quit game by changing the state to exit
void GameManager::ExitGame()
{
	_gameState = Exiting;	
}

//Game has been won
void GameManager::WinGame(int winningPlayer)
{

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


const GameObjectManager& GameManager::GetGameObjectManager()
{
	return GameManager::_gameObjectManager;
}

//Global statics
GameManager::GameState GameManager::_gameState = Uninitialized;
sf::RenderWindow GameManager::_mainWindow;
GameObjectManager GameManager::_gameObjectManager;
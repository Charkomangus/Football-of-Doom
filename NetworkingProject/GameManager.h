/*********************************************************************************
* Central project class - manages gameplay, networking, everything really
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Player.h"
#include "GameObjectManager.h"
#include "Networking.h"


class GameManager
{

public:
	static void Start();	
	static sf::RenderWindow& GetWindow();
	const static sf::Event& GetInput();
	const static GameObjectManager& GetGameObjectManager();
	const static int SCREEN_WIDTH = 1280;
	const static int SCREEN_HEIGHT = 720;
	enum GameState
	{
		Uninitialized, ShowingSplash, Paused,
		ShowingMenu, PlayingServer, PlayingClient, Exiting, Win
	};

	static GameState _gameState;
	static void ExitGame();	
	static void WinGame(int winningPlayer);
	static void ShowWin();
	static void SetState(int state);
	static void Restart();
private:
	static bool IsExiting();
	static void GameUpdate(Networking* _networking);
	static void GameDraw(Networking* _networking);
	static void ShowMenu();
	
	static void ShowSplashScreen(std::string filename);

	static GameObjectManager _gameObjectManager;
	static sf::RenderWindow _mainWindow;
};

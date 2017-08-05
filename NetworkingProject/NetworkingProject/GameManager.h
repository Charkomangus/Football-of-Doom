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
	enum GameState {
		Uninitialized, ShowingSplash, Paused,
		ShowingMenu, PlayingServer, PlayingClient, Exiting
	};

	static GameState _gameState;
private:
	static bool IsExiting();
	static void GameLoop(Networking* _networking);

	static void ShowSplashScreen();
	static void ShowMenu();	


	static sf::RenderWindow _mainWindow;
	static GameObjectManager _gameObjectManager;	
	
};

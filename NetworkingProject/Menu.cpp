//Game main menu

#include "stdafx.h"
#include "Menu.h"


Menu::MenuResult Menu::Show(sf::RenderWindow& window)
{
	//Load Image from the file
	sf::Texture image;
	image.loadFromFile("images/mainmenu.png");
	sf::Sprite sprite(image);

	//Setup Clickable regions

	//Play button
	MenuItem ServerButton;
	ServerButton.rect = sf::Rect<int>(200, 100, 1023, 235);
	ServerButton.action = PlayServer;
	
	//Exit button
	MenuItem clientButton;
	clientButton.rect = sf::Rect<int>(200, 300, 1023, 177);
	clientButton.action = PlayClient;

	//Exit button
	MenuItem exitButton;
	exitButton.rect = sf::Rect<int>(200, 500, 1023, 177);
	exitButton.action = Exit;

	_menuItems.push_back(ServerButton);
	_menuItems.push_back(clientButton);
	_menuItems.push_back(exitButton);

	window.draw(sprite);
	window.display();

	return GetMenuResponse(window);
}

//If user clicks in one of the menu items process its assigned action. Otherwise return Nothing
Menu::MenuResult Menu::HandleClick(int x, int y)
{
	for (std::list<MenuItem>::iterator it = _menuItems.begin(); it != _menuItems.end(); ++it)
	{
		sf::Rect<int> menuItemRect = (*it).rect;
		if (menuItemRect.contains(x, y)) 
		{
			std::cout << "Menu button " << (*it).action << " was pressed." << std::endl;;
			return (*it).action;
		}
	}
	return Nothing;
}

Menu::MenuResult Menu::GetMenuResponse(sf::RenderWindow& renderWindow)
{
	sf::Event menuEvent;

	while(true)
	{
		while(renderWindow.pollEvent(menuEvent))
		{
			if(menuEvent.type == sf::Event::MouseButtonPressed)
			{
				return HandleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y);
			}
			if (menuEvent.type == sf::Event::Closed)
			{
				return Exit;
			}
		}
	}
}





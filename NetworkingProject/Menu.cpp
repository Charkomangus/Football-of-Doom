/*********************************************************************************
* Game main menu class - Player chooses if game is server, client or can exit
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#include "stdafx.h"
#include "Menu.h"


Menu::MenuResult Menu::Show(sf::RenderWindow& window)
{
	//Load Image from the file
	sf::Texture image;
	image.loadFromFile(imageName);
	sf::Sprite sprite(image);

	//Setup Clickable regions

	//Play button
	MenuItem ServerButton;
	ServerButton.rect = sf::Rect<int>(0, 75, 1023, 177);
	ServerButton.imageName = "images/ServerLoading.png";
	ServerButton.action = PlayServer;
	
	//Client button
	MenuItem clientButton;
	clientButton.rect = sf::Rect<int>(250, 250, 1023, 177);
	clientButton.imageName = "images/ClientLoading.png";
	clientButton.action = PlayClient;
	
	//Exit button
	MenuItem exitButton;
	exitButton.rect = sf::Rect<int>(400, 500, 1023, 177);
	exitButton.action = Exit;

	_menuItems.push_back(ServerButton);
	_menuItems.push_back(clientButton);
	_menuItems.push_back(exitButton);

	window.draw(sprite);
	window.display();

	return GetMenuResponse(window);
}

//If user clicks in one of the menu items process its assigned action. Otherwise return Nothing
Menu::MenuResult Menu::HandleClick(int x, int y, sf::RenderWindow& renderWindow)
{
	for (std::list<MenuItem>::iterator it = _menuItems.begin(); it != _menuItems.end(); ++it)
	{
		sf::Rect<int> menuItemRect = (*it).rect;
		if (menuItemRect.contains(x, y)) 
		{
			std::cout << "Menu button " << (*it).action << " was pressed." << std::endl;
			imageName = (*it).imageName;
			sf::Texture image;
			image.loadFromFile(imageName);
			sf::Sprite sprite(image);
			renderWindow.draw(sprite);
			renderWindow.display();
			return (*it).action;
		}
	}
	return Nothing;
}

//If the mosue is pressed cal lhandle click
Menu::MenuResult Menu::GetMenuResponse(sf::RenderWindow& renderWindow)
{
	sf::Event menuEvent;

	while(true)
	{
		while(renderWindow.pollEvent(menuEvent))
		{
			if(menuEvent.type == sf::Event::MouseButtonPressed)
			{
				return HandleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y, renderWindow);
			}
			if (menuEvent.type == sf::Event::Closed)
			{
				return Exit;
			}
		}
	}
}





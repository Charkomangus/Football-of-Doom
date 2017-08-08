/*********************************************************************************
* Game main menu class - Player chooses if game is server, client or can exit
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#pragma once
#include "stdafx.h"
class Menu
{
public:
	enum MenuResult { Nothing, Exit, PlayServer, PlayClient };

	struct MenuItem
	{
	public:
		sf::Rect<int> rect;		
		MenuResult action;
		sf::String imageName;
	};

	MenuResult Show(sf::RenderWindow& renderWindow);

private:
	MenuResult GetMenuResponse(sf::RenderWindow& window);
	MenuResult HandleClick(int x, int y, sf::RenderWindow& renderWindow);
	std::list<MenuItem> _menuItems;
	sf::String imageName = "images/MainMenu.png";
};


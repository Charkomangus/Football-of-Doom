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
			
	};

	MenuResult Show(sf::RenderWindow& renderWindow);

private:
	MenuResult GetMenuResponse(sf::RenderWindow& window);
	MenuResult HandleClick(int x, int y);
	std::list<MenuItem> _menuItems;
};


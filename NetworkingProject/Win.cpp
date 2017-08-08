/*********************************************************************************
* Intro screen for showcasing logo - will display an image until the player clicks away
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/

#include "stdafx.h"
#include "Win.h"

//Display the win scren
void Win::Show(sf::RenderWindow & renderWindow)
{
	sf::Texture image;
	std::string filename = "images/Win.png";	
		
	if (image.loadFromFile(filename) != true)
	{
		return;
	}
	
	sf::Sprite sprite(image);
	renderWindow.draw(sprite);
	renderWindow.display();

	//Input - switch off application
	sf::Event event;
	while (true)
	{
		while (renderWindow.pollEvent(event))
		{		
			if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::MouseButtonPressed || event.type == sf::Event::EventType::Closed)
			{
				GameManager::ExitGame();
				renderWindow.close();
				exit(0);
			}
		}
	}

}


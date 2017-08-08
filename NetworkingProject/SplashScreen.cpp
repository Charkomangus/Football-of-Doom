/*********************************************************************************
* Intro screen for showcasing logo - will display an image until the player clicks away
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/

#include "stdafx.h"
#include "SplashScreen.h"

//Display the image if it exists and listen for input
void SplashScreen::Show(sf::RenderWindow & renderWindow, std::string filename)
{
	sf::Texture image;
	if (image.loadFromFile(filename) != true)
	{
		return;
	}

	sf::Sprite sprite(image);
	renderWindow.draw(sprite);
	renderWindow.display();

	//Input
	sf::Event event;	
	while (true)
	{
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::MouseButtonPressed || event.type == sf::Event::EventType::Closed)
			{
				return;
			}
		}
	}

}


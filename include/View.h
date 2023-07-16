#pragma once
#include <SFML/Graphics.hpp>

class View {
public:
	View(sf::VideoMode videoMode,const sf::String& title);
	void Display();
private:
	sf::RenderWindow window;
};
#pragma once
#include <vector>

#include <SFML/Graphics.hpp>
#include <Tile.h>

class View {
public:
	View(sf::VideoMode videoMode,const sf::String& title);
	void Display(std::vector<std::vector<Tile>>& Board);
private:
	sf::RenderWindow window;
	sf::View m_view;
};
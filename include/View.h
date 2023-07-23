#pragma once
#include <vector>

#include <SFML/Graphics.hpp>
#include <Tile.h>

constexpr auto VIEW_HEIGHT = 800.0f;
constexpr auto VIEW_WIDTH = 800.0f;
class View {
public:
	View(sf::VideoMode videoMode,const sf::String& title);
	sf::View getView();
	void ReSizeView();
	void Display(const std::vector<std::vector<Tile>>& Board);
	sf::RenderWindow& getWindow();
private:
	sf::RenderWindow window;
	sf::View m_view;
};
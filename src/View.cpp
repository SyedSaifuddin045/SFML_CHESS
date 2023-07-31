#include "View.h"
#include <iostream>

View::View(sf::VideoMode videoMode, const sf::String& title)
	:window(videoMode, title,sf::Style::Close | sf::Style::Resize,sf::ContextSettings((0, 0, 0, 3, 3, 0, false))), m_view(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT))
{
}

sf::View View::getView()
{
	return this->m_view;
}

void View::ReSizeView()
{
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	m_view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}

void View::Display(const std::vector<std::vector<Tile>>& Board)
{
	for (auto Tiles : Board)
	{
		for (auto tile : Tiles)
		{
			tile.Render(window);
		}
	}
}

sf::RenderWindow& View::getWindow()
{
	return this->window;
}

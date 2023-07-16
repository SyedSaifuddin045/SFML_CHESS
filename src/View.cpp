#include "View.h"
#include <iostream>

View::View(sf::VideoMode videoMode, const sf::String& title)
	:window(videoMode,title),m_view(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), sf::Vector2f(700.0f, 700.0f))
{
}

void View::Display(std::vector<std::vector<Tile>>& Board)
{
    window.setView(m_view);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for (auto Tiles : Board)
        {
            for (auto tile : Tiles)
            {
                tile.Render(window);
            }
        }
        window.display();
    }
}
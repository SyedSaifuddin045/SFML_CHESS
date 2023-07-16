#include "View.h"

View::View(sf::VideoMode videoMode, const sf::String& title)
	:window(videoMode,title)
{
}

void View::Display()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(220,100,50));
        window.display();
    }
}
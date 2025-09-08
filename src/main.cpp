#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    // SFML 3.0 uses different VideoMode constructor
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Locked and Flow");
    window.setFramerateLimit(60);

    std::cout << "Locked and Flow - SFML 3.0 Window Created" << std::endl;

    // Main loop
    while (window.isOpen())
    {
        // SFML 3.0 event handling with std::optional
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Clear screen
        window.clear(sf::Color::Black);

        // Draw here (nothing yet)

        // Display
        window.display();
    }

    return 0;
}
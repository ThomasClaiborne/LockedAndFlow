#include <SFML/Graphics.hpp>
#include <iostream>
#include "Timer.h"
#include "TimerDisplay.h"

int main()
{
    // SFML 3.0 uses different VideoMode constructor
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Locked and Flow - Timer Demo");
    window.setFramerateLimit(60);

    // Create timer and display components
    LockedAndFlow::Timer timer;
    LockedAndFlow::TimerDisplay timerDisplay(sf::Vector2f(250.0f, 200.0f));

    // Set up timer callback for real-time updates
    timer.setUpdateCallback([&](LockedAndFlow::Timer::Duration elapsed) {
        // This callback gets called whenever timer state changes
        // Could be used for logging, notifications, etc.
        static auto lastSecond = elapsed.count() / 1000;
        auto currentSecond = elapsed.count() / 1000;

        if (currentSecond != lastSecond && timer.isRunning()) {
            std::cout << "Timer running: " << currentSecond << " seconds\n";
            lastSecond = currentSecond;
        }
        });

    // Instructions text
    sf::Font instructionsFont;
    // Using empty font - in production you'd load a proper font file

    sf::Text instructions(instructionsFont);
    instructions.setCharacterSize(16);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition(sf::Vector2f(50.0f, 50.0f));
    instructions.setString("Controls:\n"
        "SPACE - Start/Resume Timer\n"
        "P - Pause Timer\n"
        "S - Stop Timer\n"
        "R - Reset Timer\n"
        "T - Set 30s Target (for testing)\n"
        "ESC - Exit");

    std::cout << "Locked and Flow Timer Demo Started" << std::endl;
    std::cout << "Use keyboard controls to interact with the timer" << std::endl;

    // Main loop
    while (window.isOpen())
    {
        // SFML 3.0 event handling with std::optional
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                std::cout << "Window closed. Final timer state: "
                    << (timer.getElapsed().count() / 1000) << " seconds" << std::endl;
                window.close();
            }

            // Handle keyboard input using SFML 3.0 pattern
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->scancode) {
                case sf::Keyboard::Scan::Space:
                    if (timer.isStopped() || timer.isPaused()) {
                        timer.start();
                        std::cout << "Timer started!" << std::endl;
                    }
                    break;

                case sf::Keyboard::Scan::P:
                    if (timer.isRunning()) {
                        timer.pause();
                        std::cout << "Timer paused at " << (timer.getElapsed().count() / 1000) << " seconds" << std::endl;
                    }
                    break;

                case sf::Keyboard::Scan::S:
                    if (!timer.isStopped()) {
                        timer.stop();
                        std::cout << "Timer stopped at " << (timer.getElapsed().count() / 1000) << " seconds" << std::endl;
                    }
                    break;

                case sf::Keyboard::Scan::R:
                    timer.reset();
                    std::cout << "Timer reset!" << std::endl;
                    break;

                case sf::Keyboard::Scan::T:
                    // Set a 30-second target for testing progress bar
                    timer.setTargetDuration(std::chrono::milliseconds(30000));
                    std::cout << "Target duration set to 30 seconds" << std::endl;
                    break;

                case sf::Keyboard::Scan::Escape:
                    std::cout << "Exit requested" << std::endl;
                    window.close();
                    break;

                default:
                    break;
                }
            }
        }

        // Update timer (handles callbacks and target duration checking)
        timer.update();

        // Update display from timer
        timerDisplay.updateFromTimer(timer);

        // Clear screen
        window.clear(sf::Color::Black);

        // Draw everything
        window.draw(instructions);
        timerDisplay.draw(window);

        // Display
        window.display();
    }


    std::cout << "Application terminated successfully" << std::endl;
    return 0;
}
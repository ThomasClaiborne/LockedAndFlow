#pragma once

#include <SFML/Graphics.hpp>
#include "Timer.h"
#include <string>
#include <memory>

namespace LockedAndFlow {

    /**
     * @brief SFML-based visual display for Timer
     *
     * Handles rendering of timer information including elapsed time,
     * progress indicators, and state visualization using SFML 3.0.0.
     * Follows modern C++ practices and SFML's new API patterns.
     */
    class TimerDisplay {
    public:
        TimerDisplay(sf::Vector2f position = { 100.0f, 100.0f });
        ~TimerDisplay() = default;

        // Display configuration
        void setPosition(sf::Vector2f position) noexcept;
        void setFont(const sf::Font& font);
        void setCharacterSize(unsigned int size) noexcept;
        void setTextColor(sf::Color color) noexcept;
        void setBackgroundColor(sf::Color color) noexcept;

        // Timer integration
        void updateFromTimer(const Timer& timer);

        // Rendering
        void draw(sf::RenderWindow& window) const;

        // Layout properties
        sf::FloatRect getBounds() const;

    private:
        // Text rendering
        sf::Text timeText_;
        sf::Text stateText_;
        sf::Text progressText_;

        // Visual elements
        sf::RectangleShape background_;
        sf::RectangleShape progressBar_;
        sf::RectangleShape progressBackground_;

        // Layout properties
        sf::Vector2f position_;
        sf::Vector2f size_;
        float padding_;

        // Helper methods
        std::string formatDuration(Timer::Duration duration) const;
        std::string stateToString(TimerState state) const;
        void updateLayout();
        void updateProgressBar(float progressPercent);

        // Default font handling
        static sf::Font& getDefaultFont();
        bool hasCustomFont_;
    };

} // namespace LockedAndFlow
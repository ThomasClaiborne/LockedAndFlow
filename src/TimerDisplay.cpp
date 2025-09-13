#include "TimerDisplay.h"
#include <sstream>
#include <iomanip>

namespace LockedAndFlow {

    TimerDisplay::TimerDisplay(sf::Vector2f position)
        : position_(position)
        , size_(sf::Vector2f(300.0f, 120.0f))
        , padding_(20.0f)
        , hasCustomFont_(false)
        , timeText_(getDefaultFont())        // SFML 3.0 requires font in constructor
        , stateText_(getDefaultFont())       // SFML 3.0 requires font in constructor  
        , progressText_(getDefaultFont()) {  // SFML 3.0 requires font in constructor

        // Configure time text
        timeText_.setCharacterSize(24);
        timeText_.setFillColor(sf::Color::White);
        timeText_.setString("00:00:00");

        // Configure state text
        stateText_.setCharacterSize(16);
        stateText_.setFillColor(sf::Color::Yellow);
        stateText_.setString("Stopped");

        // Configure progress text
        progressText_.setCharacterSize(14);
        progressText_.setFillColor(sf::Color::Cyan);
        progressText_.setString("Progress: 0%");

        // Initialize visual elements
        background_.setSize(size_);
        background_.setFillColor(sf::Color(50, 50, 50, 200));
        background_.setOutlineColor(sf::Color::White);
        background_.setOutlineThickness(2.0f);

        // Initialize progress background
        sf::Vector2f progressBgSize(size_.x - (2.0f * padding_), 10.0f);
        progressBackground_.setSize(progressBgSize);
        progressBackground_.setFillColor(sf::Color(100, 100, 100));

        // Initialize progress bar
        sf::Vector2f progressBarSize(0.0f, 10.0f);
        progressBar_.setSize(progressBarSize);
        progressBar_.setFillColor(sf::Color::Green);

        updateLayout();
    }

    void TimerDisplay::setPosition(sf::Vector2f position) noexcept {
        position_ = position;
        updateLayout();
    }

    void TimerDisplay::setFont(const sf::Font& font) {
        timeText_.setFont(font);
        stateText_.setFont(font);
        progressText_.setFont(font);
        hasCustomFont_ = true;
        updateLayout();
    }

    void TimerDisplay::setCharacterSize(unsigned int size) noexcept {
        timeText_.setCharacterSize(size);
        stateText_.setCharacterSize(static_cast<unsigned int>(size * 0.67f));
        progressText_.setCharacterSize(static_cast<unsigned int>(size * 0.58f));
        updateLayout();
    }

    void TimerDisplay::setTextColor(sf::Color color) noexcept {
        timeText_.setFillColor(color);
    }

    void TimerDisplay::setBackgroundColor(sf::Color color) noexcept {
        background_.setFillColor(color);
    }

    void TimerDisplay::updateFromTimer(const Timer& timer) {
        // Update time display
        timeText_.setString(formatDuration(timer.getElapsed()));

        // Update state display
        stateText_.setString(stateToString(timer.getState()));

        // Update state color based on timer state
        switch (timer.getState()) {
        case TimerState::Running:
            stateText_.setFillColor(sf::Color::Green);
            break;
        case TimerState::Paused:
            stateText_.setFillColor(sf::Color::Yellow);
            break;
        case TimerState::Stopped:
            stateText_.setFillColor(sf::Color::Red);
            break;
        }

        // Update progress display if target duration is set
        if (timer.getTargetDuration().has_value()) {
            const float progress = timer.getProgressPercent();
            progressText_.setString("Progress: " + std::to_string(static_cast<int>(progress)) + "%");
            updateProgressBar(progress);
        }
        else {
            progressText_.setString("No target set");
            updateProgressBar(0.0f);
        }
    }

    void TimerDisplay::draw(sf::RenderWindow& window) const {
        window.draw(background_);
        window.draw(progressBackground_);
        window.draw(progressBar_);
        window.draw(timeText_);
        window.draw(stateText_);
        window.draw(progressText_);
    }

    sf::FloatRect TimerDisplay::getBounds() const {
        return sf::FloatRect{ position_, size_ };
    }

    std::string TimerDisplay::formatDuration(Timer::Duration duration) const {
        const auto totalSeconds = duration.count() / 1000;
        const auto hours = totalSeconds / 3600;
        const auto minutes = (totalSeconds % 3600) / 60;
        const auto seconds = totalSeconds % 60;

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds;

        return oss.str();
    }

    std::string TimerDisplay::stateToString(TimerState state) const {
        switch (state) {
        case TimerState::Running: return "Running";
        case TimerState::Paused:  return "Paused";
        case TimerState::Stopped: return "Stopped";
        default: return "Unknown";
        }
    }

    void TimerDisplay::updateLayout() {
        // Position background
        background_.setPosition(position_);

        // Position time text (centered horizontally, top area)
        const auto timeBounds = timeText_.getLocalBounds();
        sf::Vector2f timePosition(
            position_.x + (size_.x - timeBounds.size.x) / 2.0f,
            position_.y + padding_
        );
        timeText_.setPosition(timePosition);

        // Position state text (left side, middle area)
        sf::Vector2f statePosition(
            position_.x + padding_,
            position_.y + padding_ + 35.0f
        );
        stateText_.setPosition(statePosition);

        // Position progress background and bar
        const float progressY = position_.y + size_.y - padding_ - 25.0f;
        sf::Vector2f progressBgPosition(position_.x + padding_, progressY);
        sf::Vector2f progressBarPosition(position_.x + padding_, progressY);

        progressBackground_.setPosition(progressBgPosition);
        progressBar_.setPosition(progressBarPosition);

        // Position progress text (right side, bottom area)
        sf::Vector2f progressTextPosition(
            position_.x + padding_,
            position_.y + size_.y - padding_ - 10.0f
        );
        progressText_.setPosition(progressTextPosition);
    }

    void TimerDisplay::updateProgressBar(float progressPercent) {
        const float maxWidth = size_.x - (2.0f * padding_);
        const float currentWidth = (progressPercent / 100.0f) * maxWidth;

        sf::Vector2f progressSize(currentWidth, 10.0f);
        progressBar_.setSize(progressSize);

        // Change color based on progress
        if (progressPercent >= 100.0f) {
            progressBar_.setFillColor(sf::Color::Red);
        }
        else if (progressPercent >= 75.0f) {
            progressBar_.setFillColor(sf::Color::Yellow);
        }
        else {
            progressBar_.setFillColor(sf::Color::Green);
        }
    }

    sf::Font& TimerDisplay::getDefaultFont() {
        static sf::Font defaultFont;
        static bool initialized = false;

        if (!initialized) {
            // Try to load a system font, fallback to empty font if none available
            initialized = true;
        }

        return defaultFont;
    }

} // namespace LockedAndFlow
#pragma once

#include <chrono>
#include <functional>
#include <optional>

namespace LockedAndFlow {

    enum class TimerState {
        Stopped,
        Running,
        Paused
    };

    /**
     * @brief High-precision timer class for productivity tracking
     *
     * Provides start/stop/pause/reset functionality with millisecond precision
     * using std::chrono. Designed for productivity applications requiring
     * accurate time measurement and state management.
     */

    class Timer {
    public:
        using Duration = std::chrono::milliseconds;
        using TimePoint = std::chrono::steady_clock::time_point;
        using TimerCallback = std::function<void(Duration)>;

        Timer();
        ~Timer() = default;

        // Core timer operations
        void start();
        void stop();
        void pause();
        void reset();

        // State queries
        TimerState getState() const noexcept { return state_; }
        bool isRunning() const noexcept { return state_ == TimerState::Running; }
        bool isPaused() const noexcept { return state_ == TimerState::Paused; }
        bool isStopped() const noexcept { return state_ == TimerState::Stopped; }

        // Time queries
        Duration getElapsed() const;
        Duration getTotalElapsed() const noexcept { return totalElapsed_; }

        // Target duration support (for future Pomodoro-style sessions)
        void setTargetDuration(Duration target) noexcept { targetDuration_ = target; }
        std::optional<Duration> getTargetDuration() const noexcept { return targetDuration_; }
        std::optional<Duration> getRemainingTime() const;
        float getProgressPercent() const;

        // Callback for real-time updates
        void setUpdateCallback(TimerCallback callback) { updateCallback_ = std::move(callback); }
        void update(); 

        // Session persistence support
        void saveElapsed(Duration elapsed) noexcept { totalElapsed_ = elapsed; }

    private:
        TimerState state_;
        TimePoint startTime_;
        Duration totalElapsed_;
        std::optional<Duration> targetDuration_;
        TimerCallback updateCallback_;

        // Internal helper methods
        void setState(TimerState newState);
        Duration getCurrentElapsed() const;
        void invokeCallback();
    };

} // namespace LockedAndFlow
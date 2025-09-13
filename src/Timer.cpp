#include "Timer.h"
#include <algorithm>

namespace LockedAndFlow {

    Timer::Timer()
        : state_(TimerState::Stopped)
        , startTime_()
        , totalElapsed_(Duration::zero())
        , targetDuration_()
        , updateCallback_() {
    }

    void Timer::start() {
        if (state_ == TimerState::Running) {
            return; // Already running
        }

        startTime_ = std::chrono::steady_clock::now();
        setState(TimerState::Running);
    }

    void Timer::stop() {
        if (state_ == TimerState::Stopped) {
            return; // Already stopped
        }

        if (state_ == TimerState::Running) {
            // Add current session time to total
            totalElapsed_ += getCurrentElapsed();
        }

        setState(TimerState::Stopped);
    }

    void Timer::pause() {
        if (state_ != TimerState::Running) {
            return; // Can only pause if running
        }

        // Save current elapsed time
        totalElapsed_ += getCurrentElapsed();
        setState(TimerState::Paused);
    }

    void Timer::reset() {
        totalElapsed_ = Duration::zero();
        setState(TimerState::Stopped);
    }

    Timer::Duration Timer::getElapsed() const {
        switch (state_) {
        case TimerState::Running:
            return totalElapsed_ + getCurrentElapsed();
        case TimerState::Paused:
        case TimerState::Stopped:
            return totalElapsed_;
        default:
            return Duration::zero();
        }
    }

    std::optional<Timer::Duration> Timer::getRemainingTime() const {
        if (!targetDuration_.has_value()) {
            return std::nullopt;
        }

        const auto elapsed = getElapsed();
        const auto target = targetDuration_.value();

        if (elapsed >= target) {
            return Duration::zero();
        }

        return target - elapsed;
    }

    float Timer::getProgressPercent() const {
        if (!targetDuration_.has_value() || targetDuration_->count() == 0) {
            return 0.0f;
        }

        const auto elapsed = getElapsed();
        const auto target = targetDuration_.value();

        const float progress = static_cast<float>(elapsed.count()) /
            static_cast<float>(target.count());

        return std::clamp(progress * 100.0f, 0.0f, 100.0f);
    }

    void Timer::update() {
        if (state_ == TimerState::Running && updateCallback_) {
            invokeCallback();
        }

        // Check if target duration reached
        if (state_ == TimerState::Running && targetDuration_.has_value()) {
            if (getElapsed() >= targetDuration_.value()) {
                stop(); // Automatically stop when target reached
            }
        }
    }

    void Timer::setState(TimerState newState) {
        if (state_ != newState) {
            state_ = newState;
            invokeCallback();
        }
    }

    Timer::Duration Timer::getCurrentElapsed() const {
        if (state_ != TimerState::Running) {
            return Duration::zero();
        }

        const auto now = std::chrono::steady_clock::now();
        const auto elapsed = std::chrono::duration_cast<Duration>(now - startTime_);
        return elapsed;
    }

    void Timer::invokeCallback() {
        if (updateCallback_) {
            updateCallback_(getElapsed());
        }
    }

} // namespace LockedAndFlow
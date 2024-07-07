#ifndef AUTO_SLEEP_H
#define AUTO_SLEEP_H

#include <Arduino.h>
#include <functional>

/**
 * @class AutoSleep
 * @brief A class to manage automatic sleep functionality.
 *
 * This class provides a way to put a system into sleep mode after a certain
 * period of inactivity. It uses two lambda functions to enable and disable
 * sleep mode, making it flexible and adaptable to different systems.
 */
class AutoSleep {
public:
    /**
     * @brief Construct a new Auto Sleep object.
     *
     * @param sleepTimeThreshold The amount of time (in milliseconds) of inactivity after which the system should go to sleep.
     * @param enableSleepFunc A lambda function that will be called to put the system into sleep mode.
     * @param disableSleepFunc A lambda function that will be called to wake the system up from sleep mode.
     */
    AutoSleep(int sleepTimeThreshold, std::function<void()> enableSleepFunc, std::function<void()> disableSleepFunc)
        : sleepTimeThreshold(sleepTimeThreshold), timeOfLastEvent(millis()), isSleeping(false),
          enableSleepFunc(enableSleepFunc), disableSleepFunc(disableSleepFunc) {
        disableSleep();
    }

    /**
     * @brief Update the time of the last event.
     *
     * This method should be called whenever an event occurs that should prevent the system from going to sleep.
     */
    void updateEventTime() {
        timeOfLastEvent = millis();
    }

    /**
     * @brief Enable sleep mode.
     *
     * This method calls the enableSleepFunc lambda function and sets the isSleeping flag to true.
     */
    void enableSleep() {
        isSleeping = true;
        enableSleepFunc();
    }

    /**
     * @brief Disable sleep mode.
     *
     * This method calls the disableSleepFunc lambda function and sets the isSleeping flag to false.
     */
    void disableSleep() {
        isSleeping = false;
        disableSleepFunc();
    }

    /**
     * @brief Check if the system should go to sleep.
     *
     * This method checks if the time since the last event is greater than the sleep time threshold. If it is, and the system is not already sleeping, it calls enableSleep().
     */
    void checkSleep() {
        unsigned long timeSinceLastEvent = millis() - timeOfLastEvent;
        if (!isSleeping && timeSinceLastEvent > sleepTimeThreshold) {
            enableSleep();
        }
    }

private:
    int sleepTimeThreshold; ///< The sleep time threshold (in milliseconds).
    unsigned long timeOfLastEvent; ///< The time (in milliseconds since the Arduino was reset) of the last event.
    bool isSleeping; ///< A flag indicating whether the system is currently sleeping.
    std::function<void()> enableSleepFunc; ///< The lambda function to call to enable sleep mode.
    std::function<void()> disableSleepFunc; ///< The lambda function to call to disable sleep mode.
};

#endif // AUTO_SLEEP_H

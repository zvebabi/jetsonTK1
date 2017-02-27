#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include <map>

enum class PIN_DIRECTION{
        INPUT_PIN=0,
        OUTPUT_PIN=1
};

enum class PIN_VALUE{
        LOW=0,
        HIGH=1
};

class GPIOController
{
public:
    GPIOController(std::string path) : sysGpioDir(path) , period(1000) , stopPWM(true) {}
    ~GPIOController();
    int initializeGPIO(uint gpioNum, PIN_DIRECTION dir, PIN_VALUE val);
    int setPinValue(PIN_VALUE val);
    void softPwmSetDC(uint dutyCycle_);
    void softPwmStart();
    void softPwmStop();
private:
    std::string sysGpioDir;
    uint pin;

    std::atomic_int dutyCycle; //%
    std::atomic_int period;    // 1000msec (freq 1kHz)
    std::atomic_bool stopPWM;
    std::thread t;
    int exportGPIO();
    int unexportGPIO();
    int setDirection(PIN_DIRECTION dir);
    int setValue(PIN_VALUE val);
};

#endif // GPIOCONTROLLER_H

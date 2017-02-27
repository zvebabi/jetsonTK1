#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H
#include <iostream>
#include <string>
#include <vector>
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
    GPIOController(std::string path) : sysGpioDir(path) {}
    ~GPIOController();
    int initializeGPIO(uint gpioNum, PIN_DIRECTION dir, PIN_VALUE val);
    int setPinValue(uint n, PIN_VALUE val);
private:
    std::string sysGpioDir;
    std::vector<uint> pins; //check if pin is init

    int exportGPIO(uint n);
    int unexportGPIO(uint n);
    int setDirection(uint n, PIN_DIRECTION dir);
    int setValue(uint n, PIN_VALUE val);
};

#endif // GPIOCONTROLLER_H

#include "GPIOController.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

GPIOController::~GPIOController()
{
    for (auto pin : pins)
        unexportGPIO(pin);
}

int GPIOController::initializeGPIO(uint gpioNum, PIN_DIRECTION dir, PIN_VALUE val)
{
    //export
    if (exportGPIO(gpioNum) <0)
        return -1;
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // wait while udev take access to gpio without root privilegies
    //set direction
    if (setDirection(gpioNum, dir) < 0)
        return -1;
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // wait while udev take access to gpio without root privilegies
    //set default value
    if (setValue(gpioNum, val) < 0)
        return -1;
    pins.push_back(gpioNum);
    return 0;
}

int GPIOController::setPinValue(uint n, PIN_VALUE val)
{
    if ( std::find(pins.begin(), pins.end(), n) != pins.end() )
        setValue(n, val);
    else
       std::cout << "Uninitialized GPIO " << n << std::endl;
    return 0;
}

int GPIOController::exportGPIO(uint n)
{
    std::ofstream f((sysGpioDir+"/export").c_str());
    if ( !f.is_open())
    {
        std::cout << "Unable to export GPIO" << n << std::endl;
        return -1;
    }
    f << std::to_string(n);
    f.close();
    return 0;
}

int GPIOController::unexportGPIO(uint n)
{
    std::ofstream f((sysGpioDir+"/unexport").c_str());
    if (!f.is_open())
    {
        std::cout << "Unable to unexport GPIO " << n << std::endl;
        return -1;
    }
    f << std::to_string(n);
    f.close();
    return 0;
}

int GPIOController::setDirection(uint n, PIN_DIRECTION dir)
{
    std::stringstream ss;
    ss << sysGpioDir << "/gpio" << n << "/direction";
    std::string path = ss.str();
    std::ofstream f(path.c_str());
    if (!f.is_open())
    {
        std::cout << "Unable to set Direction of GPIO " << n << std::endl;
        return -1;
    }
    if(dir == PIN_DIRECTION::OUTPUT_PIN)
        f << "out";
    else
        f << "in";
    f.close();
    return 0;
}

int GPIOController::setValue(uint n, PIN_VALUE val)
{
    std::stringstream ss;
    ss << sysGpioDir << "/gpio" << n << "/value";
    std::string path = ss.str();
    std::ofstream f(path.c_str());
    if (!f.is_open())
    {
        std::cout << "Unable to set Direction of GPIO " << n << std::endl;
        return -1;
    }
    f << std::to_string((int)val);
    f.close();
    return 0;
}

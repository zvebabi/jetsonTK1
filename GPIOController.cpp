#include "GPIOController.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

GPIOController::~GPIOController()
{
    unexportGPIO();
}

int GPIOController::initializeGPIO(uint gpioNum, PIN_DIRECTION dir, PIN_VALUE val)
{
    //export
    if (exportGPIO() <0)
        return -1;
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // wait while udev take access to gpio without root privilegies
    //set direction
    if (setDirection(dir) < 0)
        return -1;
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // wait while udev take access to gpio without root privilegies
    //set default value
    if (setValue(val) < 0)
        return -1;
    return 0;
}

int GPIOController::setPinValue(PIN_VALUE val)
{
    setValue(val);
}

void GPIOController::softPwmSetDC(uint dutyCycle_)
{
    dutyCycle = dutyCycle_;
}

void GPIOController::softPwmStart()
{
    stopPWM = false;
    auto a = [&]()
    {
        while(!stopPWM)
        {
            setValue(PIN_VALUE::HIGH);
            std::this_thread::sleep_for(std::chrono::milliseconds( dutyCycle * period / 100) );
            setValue(PIN_VALUE::LOW);
            std::this_thread::sleep_for(std::chrono::milliseconds( (100 - dutyCycle) * period / 100) );
        }
    };
    t = std::thread(a);
}

void GPIOController::softPwmStop()
{
    std::cout << "Stop PWM...";
    stopPWM = true;
    t.join();
    std::cout << "stopped.\n";
}

int GPIOController::exportGPIO()
{
    std::ofstream f((sysGpioDir+"/export").c_str());
    if ( !f.is_open())
    {
        std::cout << "Unable to export GPIO" << pin << std::endl;
        return -1;
    }
    f << std::to_string(pin);
    f.close();
    return 0;
}

int GPIOController::unexportGPIO()
{
    std::ofstream f((sysGpioDir+"/unexport").c_str());
    if (!f.is_open())
    {
        std::cout << "Unable to unexport GPIO " << pin << std::endl;
        return -1;
    }
    f << std::to_string(pin);
    f.close();
    return 0;
}

int GPIOController::setDirection(PIN_DIRECTION dir)
{
    std::stringstream ss;
    ss << sysGpioDir << "/gpio" << pin << "/direction";
    std::string path = ss.str();
    std::ofstream f(path.c_str());
    if (!f.is_open())
    {
        std::cout << "Unable to set Direction of GPIO " << pin << std::endl;
        return -1;
    }
    if(dir == PIN_DIRECTION::OUTPUT_PIN)
        f << "out";
    else
        f << "in";
    f.close();
    return 0;
}

int GPIOController::setValue(PIN_VALUE val)
{
    std::stringstream ss;
    ss << sysGpioDir << "/gpio" << pin << "/value";
    std::string path = ss.str();
    std::ofstream f(path.c_str());
    if (!f.is_open())
    {
        std::cout << "Unable to set Direction of GPIO " << pin << std::endl;
        return -1;
    }
    f << std::to_string((int)val);
    f.close();
    return 0;
}

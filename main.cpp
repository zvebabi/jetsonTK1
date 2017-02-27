#include <iostream>
#include <thread>
#include <chrono>

#include "GPIOController.h"

const uint LCD_PIN = 57;

int main(/*int argc, char *argv[]*/)
{
    GPIOController gpio57(std::string("/sys/class/gpio/"));
    gpio57.initializeGPIO(LCD_PIN, PIN_DIRECTION::OUTPUT_PIN, PIN_VALUE::LOW);

#if 0
    for(int i =0 ; i<5; i++)
    {
        std::cout << "Set the LED on\n";
        gpio57.setPinValue(PIN_VALUE::HIGH);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Set the LED off\n";
        gpio57.setPinValue(PIN_VALUE::LOW);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    }
#else
    gpio57.softPwmStart();
    for(int i =0 ; i<6; i++)
    {
        std::cout << "set Duty to " << 20*i << std::endl;
        gpio57.softPwmSetDC(20*i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    gpio57.softPwmStop();
#endif


    return 0;
}

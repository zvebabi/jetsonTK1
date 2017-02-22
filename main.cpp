#include <iostream>
#include <thread>
#include <chrono>

#include "GPIOController.h"

const uint LCD_PIN =57;

int main(int argc, char *argv[])
{
    GPIOController ourGPIO(std::string("/sys/class/gpio/"));
    ourGPIO.initializeGPIO(LCD_PIN, PIN_DIRECTION::OUTPUT_PIN, PIN_VALUE::LOW);

    for(int i =0 ; i<5; i++)
    {
        std::cout << "Set the LED on\n";
        ourGPIO.setPinValue(LCD_PIN, PIN_VALUE::HIGH);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "Set the LED off\n";
        ourGPIO.setPinValue(LCD_PIN, PIN_VALUE::LOW);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }


    return 0;
}

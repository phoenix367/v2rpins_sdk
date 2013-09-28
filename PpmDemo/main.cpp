/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 20:42
 */

#include <iostream>
#include <unistd.h>
#include "pincontroller/pincontroller.hpp"

using namespace std;

/*
 * Данный пример демонстрирует использование V2R pins SDK
 * для управления сервоприводом.
 */
int main(int argc, char** argv) 
{
    try
    {
        // Создаем объект для управления сервоприводом,
        // который должен быть поключен к нулевому каналу
        // аппаратного PWM
        pc::ServoRotator rotator(pc::PWM_CHANNEL::PWM_0);
        
        // Поворачиваем сервопривод на 120 градусов и ждем 5 секунд
        rotator.rotate(120);
        sleep(5);
        
        // Возвращаем привод в исходное положение и ждем 5 секунд
        rotator.rotate(0);
        sleep(5);
    }
    catch (pc::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}

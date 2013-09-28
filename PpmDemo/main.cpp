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
 * 
 */
int main(int argc, char** argv) 
{
    try
    {
        // Создаем объект для управления нулевым каналом PWM
        pc::PWM pwm1(pc::PWM_CHANNEL::PWM_0);
        
        // Устанавливаем параметры выдаваемых импульсов:
        // Длительность: 2200 микросекунд
        // Период:       20000 микросекунд
        pwm1.setPulseParams(2200, 20000);
        
        // Передаем параметры генерации в PWM
        pwm1.init();
        
        // Ждем 5 секунд и завершаем работу. Генерация импульсов
        // на нулевом канале PWM будет остановлена автоматически.
        sleep(5);
    }
    catch (pc::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}

#include "global.hpp"
#include "exceptions.hpp"

namespace pc
{
    GPIO_PIN gpioPinFromIndex(int index)
    {
        GPIO_PIN pin;
        
        switch (index)
        {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
            case 32:
            case 33:
            case 34:
            case 35:
            case 36:
            case 37:
            case 44:
            case 45:
            case 46:
            case 47:
            case 48:
            case 49:
            case 50:
            case 79:
            case 80:
            case 81:
            case 82:
            case 83:
            case 84:
            case 85:
            case 86:
            case 87:
            case 88:
            case 89:
            case 90:
            case 91:
            case 92:
                pin = (GPIO_PIN) index;
                break;
            default:
                PC_EXCEPTION(IncorrectParamException, "Invalid GPIO pin");
        }
        
        return pin;
    }
}

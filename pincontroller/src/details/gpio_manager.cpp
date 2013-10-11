#include "gpio_manager.hpp"
#include "pincontroller/exceptions.hpp"

namespace pc
{
    std::unique_ptr<GPIOManager> GPIOManager::instance;
    
    const std::unordered_map<GPIO_PIN, Contact> GPIOManager::gpioMap =
    {
        { GPIO_PIN::gpio1, Contact::con1_25 },
        { GPIO_PIN::gpio2, Contact::con1_38 },
        { GPIO_PIN::gpio3, Contact::con1_54 },
        { GPIO_PIN::gpio4, Contact::con1_55 },
        { GPIO_PIN::gpio5, Contact::con1_56 },
        { GPIO_PIN::gpio6, Contact::con1_57 },
        { GPIO_PIN::gpio7, Contact::con1_58 },
        { GPIO_PIN::gpio8, Contact::con1_59 },
        { GPIO_PIN::gpio9, Contact::con1_60 },
        { GPIO_PIN::gpio10, Contact::con1_15 },
        { GPIO_PIN::gpio11, Contact::con1_14 },
        { GPIO_PIN::gpio12, Contact::con1_13 },
        { GPIO_PIN::gpio13, Contact::con1_12 },
        { GPIO_PIN::gpio14, Contact::con1_11 },
        { GPIO_PIN::gpio15, Contact::con1_10 },
        { GPIO_PIN::gpio22, Contact::con1_41 },
        { GPIO_PIN::gpio23, Contact::con1_40 },
        { GPIO_PIN::gpio24, Contact::con1_39 },
        { GPIO_PIN::gpio25, Contact::con1_84 },
        { GPIO_PIN::gpio26, Contact::con1_37 },
        { GPIO_PIN::gpio27, Contact::con1_36 },
        { GPIO_PIN::gpio28, Contact::con1_35 },
        { GPIO_PIN::gpio29, Contact::con1_34 },
        { GPIO_PIN::gpio30, Contact::con1_33 },
        { GPIO_PIN::gpio31, Contact::con1_32 },
        { GPIO_PIN::gpio32, Contact::con1_31 },
        { GPIO_PIN::gpio33, Contact::con1_30 },
        { GPIO_PIN::gpio34, Contact::con1_85 },
        { GPIO_PIN::gpio35, Contact::con1_81 },
        { GPIO_PIN::gpio36, Contact::con1_27 },
        { GPIO_PIN::gpio37, Contact::con1_26 },
        { GPIO_PIN::gpio44, Contact::con1_80 },
        { GPIO_PIN::gpio45, Contact::con1_79 },
        { GPIO_PIN::gpio46, Contact::con1_78 },
        { GPIO_PIN::gpio47, Contact::con1_77 },
        { GPIO_PIN::gpio48, Contact::con1_76 },
        { GPIO_PIN::gpio49, Contact::con1_75 },
        { GPIO_PIN::gpio50, Contact::con1_20 },
        { GPIO_PIN::gpio79, Contact::con1_62 },
        { GPIO_PIN::gpio80, Contact::con1_42 },
        { GPIO_PIN::gpio81, Contact::con1_65 },
        { GPIO_PIN::gpio82, Contact::con1_61 },
        { GPIO_PIN::gpio83, Contact::con1_83 },
        { GPIO_PIN::gpio84, Contact::con1_82 },
        { GPIO_PIN::gpio85, Contact::con1_64 },
        { GPIO_PIN::gpio86, Contact::con1_63 },
        { GPIO_PIN::gpio87, Contact::con1_19 },
        { GPIO_PIN::gpio88, Contact::con1_18 },
        { GPIO_PIN::gpio89, Contact::con1_17 },
        { GPIO_PIN::gpio90, Contact::con1_16 },
        { GPIO_PIN::gpio91, Contact::con1_44 },
        { GPIO_PIN::gpio92, Contact::con1_43 }
    };
    
    GPIOManager::GPIOManager()
    {
        contacts = {
            {Contact::con1_1, ContactState::notAvailable},
            {Contact::con1_2, ContactState::notAvailable},
            {Contact::con1_3, ContactState::notAvailable},
            {Contact::con1_4, ContactState::notAvailable},
            {Contact::con1_5, ContactState::notAvailable},
            {Contact::con1_6, ContactState::notAvailable},
            {Contact::con1_7, ContactState::notAvailable},
            {Contact::con1_8, ContactState::notAvailable},
            {Contact::con1_9, ContactState::notAvailable},
            {Contact::con1_10, ContactState::notAquired},
            {Contact::con1_11, ContactState::notAquired},
            {Contact::con1_12, ContactState::notAquired},
            {Contact::con1_13, ContactState::notAquired},
            {Contact::con1_14, ContactState::notAquired},
            {Contact::con1_15, ContactState::notAquired},
            {Contact::con1_16, ContactState::notAquired},
            {Contact::con1_17, ContactState::notAquired},
            {Contact::con1_18, ContactState::notAquired},
            {Contact::con1_19, ContactState::notAquired},
            {Contact::con1_20, ContactState::notAquired},
            {Contact::con1_21, ContactState::notAvailable},
            {Contact::con1_22, ContactState::notAvailable},

            {Contact::con1_23, ContactState::notAvailable},
            {Contact::con1_24, ContactState::notAvailable},
            {Contact::con1_25, ContactState::notAquired},
            {Contact::con1_26, ContactState::notAquired},
            {Contact::con1_27, ContactState::notAquired},
            {Contact::con1_28, ContactState::notAquired},
            {Contact::con1_29, ContactState::notAquired},
            {Contact::con1_30, ContactState::notAquired},
            {Contact::con1_31, ContactState::notAquired},
            {Contact::con1_32, ContactState::notAquired},
            {Contact::con1_33, ContactState::notAquired},
            {Contact::con1_34, ContactState::notAquired},
            {Contact::con1_35, ContactState::notAquired},
            {Contact::con1_36, ContactState::notAquired},
            {Contact::con1_37, ContactState::notAquired},
            {Contact::con1_38, ContactState::notAquired},
            {Contact::con1_39, ContactState::notAquired},
            {Contact::con1_40, ContactState::notAquired},
            {Contact::con1_41, ContactState::notAquired},
            {Contact::con1_42, ContactState::notAquired},
            {Contact::con1_43, ContactState::notAquired},
            {Contact::con1_44, ContactState::notAquired},

            {Contact::con1_45, ContactState::notAvailable},
            {Contact::con1_46, ContactState::notAvailable},
            {Contact::con1_47, ContactState::notAvailable},
            {Contact::con1_48, ContactState::notAvailable},
            {Contact::con1_49, ContactState::notAvailable},
            {Contact::con1_50, ContactState::notAvailable},
            {Contact::con1_51, ContactState::notAvailable},
            {Contact::con1_52, ContactState::notAvailable},
            {Contact::con1_53, ContactState::notAvailable},
            {Contact::con1_54, ContactState::notAquired},
            {Contact::con1_55, ContactState::notAquired},
            {Contact::con1_56, ContactState::notAquired},
            {Contact::con1_57, ContactState::notAquired},
            {Contact::con1_58, ContactState::notAquired},
            {Contact::con1_59, ContactState::notAquired},
            {Contact::con1_60, ContactState::notAquired},
            {Contact::con1_61, ContactState::notAquired},
            {Contact::con1_62, ContactState::notAquired},
            {Contact::con1_63, ContactState::notAquired},
            {Contact::con1_64, ContactState::notAquired},
            {Contact::con1_65, ContactState::notAquired},

            {Contact::con1_66, ContactState::notAvailable},
            {Contact::con1_67, ContactState::notAvailable},
            {Contact::con1_68, ContactState::notAvailable},
            {Contact::con1_69, ContactState::notAvailable},
            {Contact::con1_70, ContactState::notAvailable},
            {Contact::con1_71, ContactState::notAvailable},
            {Contact::con1_72, ContactState::notAvailable},
            {Contact::con1_73, ContactState::notAvailable},
            {Contact::con1_74, ContactState::notAvailable},
            {Contact::con1_75, ContactState::notAquired},
            {Contact::con1_76, ContactState::notAquired},
            {Contact::con1_77, ContactState::notAquired},
            {Contact::con1_78, ContactState::notAquired},
            {Contact::con1_79, ContactState::notAquired},
            {Contact::con1_80, ContactState::notAquired},
            {Contact::con1_81, ContactState::notAquired},
            {Contact::con1_82, ContactState::notAquired},
            {Contact::con1_83, ContactState::notAquired},
            {Contact::con1_84, ContactState::notAquired},
            {Contact::con1_85, ContactState::notAquired},
            {Contact::con1_86, ContactState::notAvailable}
        };
    }
    
    GPIOManager::~GPIOManager()
    {
        
    }
    
    GPIOManager* GPIOManager::getInstance()
    {
        if (!instance)
        {
            instance = std::unique_ptr<GPIOManager>(new GPIOManager());
        }
        
        return instance.get();
    }

    void GPIOManager::aquire(Contact c)
    {
        try
        {
            if (contacts.at(c) == ContactState::notAvailable ||
                contacts.at(c) == ContactState::aquired)
            {
                PC_EXCEPTION(PinLockedException, "Can't aquire pin.");
            }
            
            contacts[c] = ContactState::aquired;
        }
        catch (std::out_of_range& e)
        {
            PC_EXCEPTION(IncorrectParamException, e.what());
        }
    }
    
    void GPIOManager::release(Contact c)
    {
        try
        {
            if (contacts.at(c) == ContactState::notAvailable)
            {
                PC_EXCEPTION(PinLockedException, "Can't release pin.");
            }
            
            contacts[c] = ContactState::notAquired;
        }
        catch (std::out_of_range& e)
        {
            PC_EXCEPTION(IncorrectParamException, e.what());
        }
    }
}

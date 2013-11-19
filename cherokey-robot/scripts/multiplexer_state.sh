#!/bin/sh

GPIO_STATES=`head -c 83 /dev/v2r_gpio`
if ([ $? -ne 0 ])
then
    echo "Error occured during read GPIO state"
    exit 1
fi

A_BUS_BIT=${GPIO_STATES:82:1}
B_BUS_BIT=${GPIO_STATES:79:1}

if [ "$A_BUS_BIT" = "1" ] && [ "$B_BUS_BIT" = "0" ]
then
    echo "A"
elif [ "$A_BUS_BIT" = "0" ] && [ "$B_BUS_BIT" = "1" ]
then
    echo "B"
elif [ "$A_BUS_BIT" = "0" ] && [ "$B_BUS_BIT" = "0" ]
then
    echo "off"
else
    echo "Invalid multiplexer state"
fi

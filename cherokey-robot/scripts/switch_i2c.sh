#!/bin/sh

if ([ $# -ne 1 ])
then
    echo "Invalid arguments. Usage: switch_i2c.sh <bus index>"
    exit 1
fi

BUS_INDEX=$1

case "$BUS_INDEX" in
"A")
    echo "set gpio79 output:0" > /dev/v2r_gpio
    echo "set gpio82 output:1" > /dev/v2r_gpio
    if ([ $? -eq 0 ])
    then
        echo "Select bus A"
    else
        echo "Error occured during bus switching"
    fi
    ;;
"B")
    echo "set gpio79 output:1" > /dev/v2r_gpio
    echo "set gpio82 output:0" > /dev/v2r_gpio
    if ([ $? -eq 0 ])
    then
        echo "Select bus B"
    else
        echo "Error occured during bus switching"
    fi
    ;;
"off")
    echo "set gpio79 output:0" > /dev/v2r_gpio
    echo "set gpio82 output:0" > /dev/v2r_gpio
    if ([ $? -eq 0 ])
    then
        echo "I2C bus multiplexor is off"
    else
        echo "Error occured during bus shutting down"
    fi
    ;;
*)
    echo "Incorrect multiplexing bus index $BUS_INDEX"
    exit 1
    ;;
esac

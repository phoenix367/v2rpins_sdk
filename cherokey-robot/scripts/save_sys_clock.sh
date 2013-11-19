#!/bin/sh

echo "Save Virt2real system time to DS1307 module..."

I2C_CREATE_DEVICE_FILE="/sys/class/i2c-adapter/i2c-1/new_device"
I2C_DELETE_DEVICE_FILE="/sys/class/i2c-adapter/i2c-1/delete_device"

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

eval "${SCRIPT_DIR}/switch_i2c.sh B"

echo "Attach to clock device..."
echo ds1307 0x68 > $I2C_CREATE_DEVICE_FILE

echo "Save system clock..."
hwclock -w -f /dev/rtc1

echo "Detach clock device"
echo 0x68 > $I2C_DELETE_DEVICE_FILE

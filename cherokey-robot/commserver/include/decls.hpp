/* 
 * File:   decls.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 12 Март 2014 г., 1:01
 */

#ifndef DECLS_HPP
#define	DECLS_HPP

enum class SensorIds : int
{
    Voltage_sensor = 0,
    GPS_sensor = 1,
    IMU_sensor = 2,
    WIFI_sensor = 3
};

#define IMU_SENSOR_NAME                 "IMU"
#define GPS_SENSOR_NAME                 "GPS"
#define VOLTAGE_SENSOR_NAME             "voltage_current"
#define WIFI_SENSOR_NAME                "WiFi"
#define WIFI_INTERFACE_NAME             "wifi_interface"

#define VOLTAGE_PARAM_VOLTAGE           "voltage"
#define VOLTAGE_PARAM_CURRENT           "current"

#define GPS_PARAM_LATITUDE              "latitude"
#define GPS_PARAM_LONGITUDE             "longitude"
#define GPS_PARAM_VELOCITY              "velocity"
#define GPS_PARAM_DIRECTION             "direction"

#define WIFI_PARAM_INTERFACE            "interface"
#define WIFI_PARAM_QUALITY              "quality"
#define WIFI_PARAM_SIGNAL_LEVEL         "sig_level"

#endif	/* DECLS_HPP */

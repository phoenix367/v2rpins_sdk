/* 
 * File:   IMUReader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 11, 2014, 1:41 AM
 */

#ifndef IMUREADER_HPP
#define	IMUREADER_HPP

#include "SensorReader.hpp"

class IMUReader : public SensorReader
{
public:
    IMUReader();
    virtual ~IMUReader();
    
protected:
    virtual void run();
    
private:
    static void writeToDevice(int file, const char * buf, int len);
    static void selectDevice(int file, int addr, const char * name);
    
    void initSensors();
    void readSensors();
    
private:
    int file;
};

#endif	/* IMUREADER_HPP */

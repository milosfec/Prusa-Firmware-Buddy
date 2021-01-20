//HardwareSerial.h - A3ides/STM32
#ifndef _HARDWARESERIAL_H
#define _HARDWARESERIAL_H

#include <inttypes.h>
#include "Stream.h"

class HardwareSerial : public Stream {
public:
    HardwareSerial(void *peripheral);
    void begin(unsigned long baud);
    void begin(unsigned long, uint8_t);
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t size);
    operator bool();
};


// ultra-mega-terrible-hack. 
class HardwareSerial2 : public Stream {
public:
    HardwareSerial2(void *p){};
    void begin(unsigned long baud);
    virtual int available(void);
    virtual int peek(void) { return -1; };
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    virtual size_t write(uint8_t *buffer, size_t size);
    operator bool() { return true;};
};

extern HardwareSerial Serial3;
extern HardwareSerial2 SerialUART1;


#endif //_HARDWARESERIAL_H

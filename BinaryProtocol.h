/*
  BinaryProtocol.h - Library for binary comunication
  Created by Filip Richter
  Released into the public domain.
*/

#ifndef BinaryProtocol_h
#define BinaryProtocol_h

#include "Arduino.h"
#define BUFFER_SIZE 256

#define ESCAPE_CHAR 27
#define START_CHAR  1
#define END_CHAR	4

#if BUFFER_SIZE > 256
  #define POINTER_TYPE int
#else
  #define POINTER_TYPE byte
#endif

class BinaryProtocol
{
  public:
    BinaryProtocol(Stream &s);
    BinaryProtocol(Stream &s, void (*callback)(byte data[], POINTER_TYPE len));
    void sendPacket(byte data[],POINTER_TYPE len);
    void sendIRQ(byte irq);
    void doWork();
    void setCallback(void (*callback)(byte data[], POINTER_TYPE len));
    void setIrqCallback(void (*callbackIrq)(byte code));
  private:
  	void parse(byte b);
  	void addByte(byte b);

    byte buffer[BUFFER_SIZE];
    POINTER_TYPE pointer; 

    bool escaped;
    bool decoding;

    Stream& _s;
    void (*_callback)(byte data[], POINTER_TYPE len);
	void (*_callbackIrq)(byte code);
};

#endif


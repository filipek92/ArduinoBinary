/*
  BinaryProtocol.cpp - Library for binary comunication
  Created by Filip Richter
  Released into the public domain.
*/

#include "BinaryProtocol.h"

BinaryProtocol::BinaryProtocol(Stream &s): _s(s)
{
  pointer=0; 

  escaped=false;
  decoding=false;

  _callback = 0;
  _callbackIrq = 0;
}

BinaryProtocol::BinaryProtocol(Stream &s, void (*callback)(byte data[], POINTER_TYPE len)): _s(s)
{
  pointer=0; 

  escaped=false;
  decoding=false;

  _callback = callback;
  _callbackIrq = 0;
}

void BinaryProtocol::sendPacket(byte data[],POINTER_TYPE len){
  _s.write(ESCAPE_CHAR);
  _s.write(START_CHAR);
  for(POINTER_TYPE i=0; i<len; i++){
    if(data[i]==ESCAPE_CHAR){
      _s.write(ESCAPE_CHAR);
    }
    _s.write(data[i]);
  }
  _s.write(ESCAPE_CHAR);
  _s.write(END_CHAR);
}

void BinaryProtocol::sendIRQ(byte irq){
  _s.write(ESCAPE_CHAR);
  _s.write(irq);
}

void BinaryProtocol::setCallback(void (*callback)(byte data[], POINTER_TYPE len)){
  _callback = callback;
}
void BinaryProtocol::setIrqCallback(void (*callbackIrq)(byte code)){
  _callbackIrq = callbackIrq;
}

void BinaryProtocol::doWork(){
  while(_s.available()){
    parse(_s.read());
  }
}

void BinaryProtocol::parse(byte b){
  if(escaped){
    switch(b){
      case START_CHAR://Start of packet
        if(decoding){
          _callback(buffer, pointer);
        }
        pointer=0;
        decoding=true;
        break;
      case END_CHAR:
        if(decoding){
          _callback(buffer, pointer);
        }
        decoding=false;
        pointer=0;
        break;
      case ESCAPE_CHAR:
        addByte(ESCAPE_CHAR);
        break;
      default:
        _callbackIrq(b);
    }
    escaped=false;
    return;
  }
  if(b==ESCAPE_CHAR){
    escaped=true;
    return;      
  }
  addByte(b);
}

void BinaryProtocol::addByte(byte b){
  if(!decoding) return;
  if(pointer>=BUFFER_SIZE) return;
  buffer[pointer]=b;
  pointer++;
}

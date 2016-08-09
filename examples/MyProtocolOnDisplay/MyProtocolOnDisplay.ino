#include <BinaryProtocol.h>

BinaryProtocol pcLink(Serial);

void initLCD();

void setup() {
  Serial.begin(115200);
  pcLink.setCallback(callback);
  pcLink.setIrqCallback(cmdIrq);
  initLCD();
}

void loop() {
  pcLink.doWork();
}

void cmdIrq(byte b){
  Serial.println();
  Serial.print("SERIAL IRQ: ");
  Serial.println(b);
  if(b==91){
    digitalWrite(13, !digitalRead(13));
  }
}

void callback(byte data[], byte len){
  Serial.print("Packet: ");
  for(byte i=0; i<len; i++){
    Serial.print(data[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
  printPacketCyclic(data, len);
  pcLink.sendPacket(data, len);
}

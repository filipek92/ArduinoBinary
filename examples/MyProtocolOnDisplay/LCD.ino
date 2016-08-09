#include <UTFT.h>

UTFT myGLCD(HX8357B, 38, 39, 40, 41);

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

void initLCD(){
  myGLCD.InitLCD();
  myGLCD.InitLCD();
  paintMainScreen();
}

void paintMainScreen() {
  int x, y;
  myGLCD.clrScr();
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 479, 18);

  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 306, 479, 319);

  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(0, 19, 479, 290);

  myGLCD.setFont(BigFont);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("Protocol Tester", CENTER, 1);

  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255, 255, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Filip Richter 2016 (c)", CENTER, 307);
  myGLCD.setFont(BigFont);
}

void printPacketCyclic(byte packet[], byte len){
  static byte line=0;
  printPacket(line, packet, len);
  line = (line+1)%14;
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(5, 20+line*18, 477, 37+line*18);
}

const char hex[] = "0123456789ABCDEF";
char ch[] = "0A";
void printPacket(byte line, byte packet[], byte len){
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(0, 255, 0);
  for(byte i=0; (i<len) && (i<12); i++){
    ch[1] = hex[packet[i]&0x0F];
    ch[0] = hex[packet[i]>>4];
    myGLCD.print(ch, 5+13*i*3, 20+line*18);
  } 
}

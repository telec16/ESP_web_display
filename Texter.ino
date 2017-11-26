#include "Fonts.h"
#include "Texter.h"

#define DISPLAY_MATRIX 3
byte buffer[8][DISPLAY_MATRIX];

void writeWord(byte addr, byte data)
{
  writeWord(addr, data, false);
}

void writeWord(byte addr, byte data, boolean hold)
{
  word sended = addr<<8 | data<<0;
  int i;

  digitalWrite(CLK,LOW);
  digitalWrite(CS,LOW);
  
  for(i=0; i<16; i++)
  {
    digitalWrite(DIN, (sended>>(15-i))&1);
    delayMicroseconds(1);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(CLK, LOW);
  }
  
  digitalWrite(CS,!hold);
  
}

void init_matrix(byte intensity)
{
  for(int j=DISPLAY_MATRIX-1; j>=0; j--)
    writeWord(0b1100, 0xFF, j>0);//shutdown
    
  for(int j=DISPLAY_MATRIX-1; j>=0; j--)
    writeWord(0b1111, 0x00, j>0);//testmode
    
  for(int j=DISPLAY_MATRIX-1; j>=0; j--)
    writeWord(0b1001, 0x00, j>0);//decodemode
    
  for(int j=DISPLAY_MATRIX-1; j>=0; j--)
    writeWord(0b1011, 0x07, j>0);//scanline
    
  for(int j=DISPLAY_MATRIX-1; j>=0; j--)
    writeWord(0b1010, intensity, j>0);
}

void writeMsg(const char msg[], int defilDelay)
{
  for(int i=0; i < (int)strlen(msg); i++)
  {
    for(int j=0; j<6; j++)
    {
      if(j<5)
        shiftBuffer(msg[i], j);
      else
        shiftBuffer(' ', 0);

      applyBuffer();
      
      delay(defilDelay);
    }
  }
}

void writeLetter(char letter)
{
  byte row;
  for(int j=0; j<7; j++)
  {
    row=0;
    
    for(int i=0; i<5; i++)
    {
      row|=((asciiTable[(letter-' ')*5+i]>>j)&1)<<i;
    }
      
    writeWord(j+1, row);
  }
}

void shiftBuffer(char nextLetter, int offset)
{
  byte slice = asciiTable[(nextLetter-' ')*5+offset];
  
  for(int i=0; i<8; i++)
  {
    for(int j=DISPLAY_MATRIX-1; j>=0; j--)
    {
      buffer[i][j]>>=1;
      if(j!=0)
        buffer[i][j]|=(buffer[i][j-1]&0x01)<<7;
      else
        buffer[i][j]|=((slice>>i)&0x01)<<7;
    }
  }  
}

void applyBuffer()
{
  for(int i=0; i<8; i++)
  {
    for(int j=DISPLAY_MATRIX-1; j>=0; j--)
    {
      writeWord(i+1, buffer[i][j], j>0);
    }
  }
}

void changeIntensity(int intensity)
{
  for(int j=DISPLAY_MATRIX-1; j>=0; j--)
  {
    writeWord(0b1010, intensity, j>0);
  }
}


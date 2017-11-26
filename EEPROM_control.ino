/*
 *  Memory organization:
 *  Index SSID \0 Password \0 SSID \0 Password \0...
 *  Index : 1Byte
 *  SSID/pass : 31Bytes + 0
 *  size: 512 bytes
 */
 
#include "EEPROM_control.h"
#include <EEPROM.h>


void init_EEPROM()
{
  EEPROM.begin(SIZE);
}

int lastUsedIDs(String *ssid, String *password)
{
  byte index = EEPROM.read(0);
  
  if(index == 0xFF)
    return -1;

  int start = INDEX_LEN + index*(SSID_LEN+PSW_LEN);
  *ssid = "";
  *password = "";
  
  for (int i = 0; i < SSID_LEN; ++i)
    *ssid += char(EEPROM.read(start + i));
  for (int i = 0; i < PSW_LEN; ++i)
    *password += char(EEPROM.read(start + i + SSID_LEN));

  return index;
}

int putNewIDs(String *ssid, String *password)
{
  byte index = 0xFF;

  //Check for free space
  for(int i=0; (i<CLUSTERS) && (index == 0xFF); ++i)
    if(EEPROM.read(INDEX_LEN + i*(SSID_LEN+PSW_LEN)) == '\0') index = i;
    
  if(index == 0xFF)
    return -1;

  int start = INDEX_LEN + index*(SSID_LEN+PSW_LEN);

  //Writting up data
  for (int i = 0; i < SSID_LEN; ++i)
  {
    char c = (i < (int)ssid->length()) ? ssid->charAt(i) : '\0';
    EEPROM.write(start + i, c);
  }
  for (int i = 0; i < PSW_LEN; ++i)
  {
    char c = (i < (int)password->length()) ? password->charAt(i) : '\0';
    EEPROM.write(start + i + SSID_LEN, c);
  }

  EEPROM.commit();

  Serial.println("NID:"+(String) index);

  return index;
}

boolean delIDs(byte index)
{
  if(EEPROM.read(0) == index) //Erase the current use
    EEPROM.write(0, 0xFF);

  int start = INDEX_LEN + index*(SSID_LEN+PSW_LEN);
  
  for (int i = 0; i < (SSID_LEN+PSW_LEN); ++i)
    EEPROM.write(start + i, '\0');

  EEPROM.commit();

  Serial.println("DEL:"+(String) index);

  return (EEPROM.read(0) != 0xFF); //If we erased the currently used
}

void eraseALL()
{
  EEPROM.write(0, 0xFF);
  for (int i = INDEX_LEN; i < SIZE; ++i)
    EEPROM.write(i, '\0');

  EEPROM.commit();

  return;
}

void listSSIDs(char ssids[CLUSTERS][SSID_LEN])
{  
  for (int i = 0; i < CLUSTERS; ++i)
  {
    //ssids[i] = i + ">";
    for(int j=0; j < SSID_LEN; ++j)
    {
      char chr = char(EEPROM.read(INDEX_LEN + i*(SSID_LEN+PSW_LEN) + j));
      ssids[i][j] = (chr!='\0') ? chr:' ';
      Serial.print(ssids[i][j]);
    }
  }
}

boolean getIDs(byte index, String *ssid, String *password)
{
  EEPROM.write(0, index); // Last used IDs
  EEPROM.commit();

  int start = INDEX_LEN + index*(SSID_LEN+PSW_LEN);
  *ssid = "";
  *password = "";
  
  for (int i = 0; i < SSID_LEN; ++i)
    *ssid += char(EEPROM.read(start + i));
  for (int i = 0; i < PSW_LEN; ++i)
    *password += char(EEPROM.read(start + i + SSID_LEN));

  return (EEPROM.read(start) != '\0');
}




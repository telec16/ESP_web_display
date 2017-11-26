#ifndef EEPROM_CONTROL_H
#define EEPROM_CONTROL_H

#define SIZE 512
#define INDEX_LEN  1
#define SSID_LEN 32
#define PSW_LEN  32
#define CLUSTERS ((SIZE-INDEX_LEN)/(SSID_LEN+PSW_LEN))


void init_EEPROM();
void eraseALL();

int lastUsedIDs(String *ssid, String *password);
int putNewIDs(String *ssid, String *password);
boolean delIDs(byte index);
void listSSIDs(char ssids[CLUSTERS][SSID_LEN]);
boolean getIDs(byte index, String *ssid, String *password);

#endif

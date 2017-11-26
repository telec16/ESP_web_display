/*
 * | v    v    v    v    v  |
 *  GND  MISO MOSI  0   RST
 *  0> LOW to program
 *  RST> Float
 *  https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/readme.md
 */
/*
 * Au lancement ou Pas de connexion : mode server avec 2*3 caractères aléatoires pour rentrer 
 * => definition du ssid et du mdp + save/load des anciens couples + scan du réseau pour aider
 * 
 * Connecté : mode client, ping périodiquement ou à chaque tape ? option en mode server ?
 * affichage défilant des messages
 * 
 * Ajout:
 * Capteurs :
 *  piezo
 *  capacitive touch
 * Actionneurs:
 *  rgb
 *  
 *  
 *  A faire:
 *  Gestion des IDs
 *  LEDs
 *  capteur pour controler
}
 */

#include <ESP8266WiFi.h>
#include "Texter.h"
#include "WifiServer.h"
#include "WifiClient.h"
#include "EEPROM_control.h"

#define CS 12
#define CLK 14
#define DIN 13

#define BTN_UP 5  //Abort connection
#define BTN_DOWN 4//Change display

//#define MSG_SIZE 80
#define MAX_TRIES 10

String ssid     = "";
String password = "";
int option = 0;

const char* host = "telec.16mb.com";
const char* page = "http://telec.16mb.com/computercraft/sender.php?msg";
const int httpPort = 80;

const char START[]="<!--Start-->";

void setup() {
  
  Serial.begin(115200);
  delay(10);
  
  Serial.println();
  Serial.println();
  Serial.println("Hi !");

  delay(10);

  pinMode(CS, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  digitalWrite(CLK,LOW);
  digitalWrite(CS,LOW);
  digitalWrite(BTN_DOWN,HIGH);
  digitalWrite(BTN_UP,HIGH);

  init_matrix(5);
  init_EEPROM();
  //eraseALL();
  
  if(lastUsedIDs(&ssid, &password) == -1)
    createServerAndSetParams(&ssid, &password, &option);
    

  while(!connectToClient(MAX_TRIES, ssid, password)){
    createServerAndSetParams(&ssid, &password, &option);
  }
}

int pos;
int displayFct = 0;
String msg="";
void connectAndShow();


void loop() {
  delay(5000);
  Serial.println("Looping..."+(String)displayFct+" "+(String)digitalRead(BTN_DOWN)+ " " +(String)digitalRead(BTN_UP));

  if(digitalRead(BTN_DOWN) == HIGH)
  {
    displayFct = !displayFct;
    if(displayFct == 0)
      writeMsg("Last", 0);
    else
      writeMsg("Stay", 0);
  }
  if(digitalRead(BTN_UP) == HIGH)
  {
    do{
      createServerAndSetParams(&ssid, &password, &option);
    }while(!connectToClient(MAX_TRIES, ssid, password));
  }
  
  if(displayFct == 0)
    connectAndShow();
  else
    writeMsg(msg.c_str(), 100);
}

void connectAndShow()
{
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("Requesting URL: ");
  Serial.println(page);
  
  // This will send the request to the server
  client.print(String("GET ") + page + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    if((pos=line.indexOf('>'))!=-1)
    {
      line=line.substring(pos+1, line.length()-1);
      msg=line+" ";
      writeMsg(msg.c_str(), 100);
    }
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}


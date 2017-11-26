/*
 * | v  v  v  v  v              |
 *  GND Rx Tx 0 RST
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
 */

#include <ESP8266WiFi.h>
#include "Texter.ino"

#define CS 12
#define CLK 14
#define DIN 13

//#define MSG_SIZE 80

String ssid     = "FREEhome";
String password = "11081105";
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
  digitalWrite(CLK,LOW);
  digitalWrite(CS,LOW);

  init_matrix(5);
  
  createServerAndSetParams(&ssid, &password, &option);
  
  connectToClient(-1, ssid, password);
}

int pos;

void loop() {
  delay(5000);

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
      writeMsg((line+" ").c_str(), 100);
    }
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}


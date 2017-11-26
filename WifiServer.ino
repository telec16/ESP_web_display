#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Texter.h"
#include "WifiServer.h"
#include "EEPROM_control.h"

#define SAVE_1 496
#define SAVE_SIZE 61


void setupWiFi();
String generatePsw();
void sad_end();
void happy_end();
void deleteIfAsked();
void resetIfAsked();


ESP8266WebServer server(80);
boolean waitingForIDs = true;
String id, psw, facePsw;
char ssids[CLUSTERS][SSID_LEN];

char htmlBody_root[] = "<h1>Please insert ids</h1><br/>\r\n"
"<form method='post' action=''>\r\n"
"<label for='face_psw'>Facade : </label>\r\n"
"<input type='text' name='face_psw' id='face_psw'/></br></br>\r\n"
"<label for='ssid'>SSID : </label>\r\n"
"<input type='text' name='ssid' id='ssid'/></br>\r\n"
"<label for='psw'>MDP : </label>\r\n"
"<input type='text' name='psw' id='psw'/></br>\r\n"
"<input type='checkbox' name='save'/>Sauvegarder</br>\r\n"
"OU</br>\r\n"
"<select name='load'><option value='' selected='selected'>Selectionnez...</option>\r\n"
"<option value='1'>                                </option>\r\n"
"<option value='2'>                                </option>\r\n"
"<option value='3'>                                </option>\r\n"
"<option value='4'>                                </option>\r\n"
"<option value='5'>                                </option>\r\n"
"<option value='6'>                                </option>\r\n"
"<option value='7'>                                </option>\r\n"
"</select></br></br>\r\n"
"Supprimer : <input type='checkbox' name='c1'/>1<input type='checkbox' name='c2'/>2\r\n"
"<input type='checkbox' name='c3'/>3<input type='checkbox' name='c4'/>4<input type='checkbox' name='c5'/>5\r\n"
"<input type='checkbox' name='c6'/>6<input type='checkbox' name='c7'/>7</br>\r\n"
"<input type='checkbox' name='reset'/>Reinitialisation</br>\r\n"
"<input id='button' type='submit' value='Envoyer' />\r\n"
"</form>";
char htmlBody_thanks[] = "<h1>Thank you, you will be disconnected...</h1>\r\n";
char htmlBody_wrong[] = "<h1>Wrong something, pls reco</h1>\r\n";
  
void handleRoot() {
   
  if (server.hasArg("ssid") && server.hasArg("psw") && server.hasArg("face_psw") && server.hasArg("load")){
      if(server.arg("face_psw") == facePsw){

        resetIfAsked();
        deleteIfAsked();
        
        if(server.arg("load") != "")
        {
          if(!getIDs((byte)(server.arg("load").c_str()[0] - '0' - 1), &id, &psw))
            sad_end();
          else
            happy_end();
        }
        else
        {
          id = server.arg("ssid");
          psw = server.arg("psw");

          if(server.hasArg("save"))
            putNewIDs(&id, &psw);

          happy_end();
        }
      }
      else{
        sad_end();
      }
  } else{
    
    listSSIDs(ssids);
    for(int i=0; i<CLUSTERS; i++)
      for(int j=0; j<SSID_LEN; j++)
        htmlBody_root[i*SAVE_SIZE + SAVE_1 + j] = ssids[i][j];
      
    server.send(200, "text/html", htmlBody_root);
    facePsw = generatePsw();
    Serial.println(facePsw);
    writeMsg("    ", 1);
    writeMsg(facePsw.c_str(), 50);
  }
}
void createServerAndSetParams(String *ssid, String *password, int *option)
{  
  WiFiClient client;
  
  setupWiFi();
  server.on("/", handleRoot);
  server.onNotFound(handleRoot);
  server.begin();
  Serial.println("Server started");
  
  waitingForIDs = true;
  while(waitingForIDs){
    server.handleClient();
  }
  Serial.println("Client disonnected");

  *ssid = id;
  *password = psw;

  return;
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);
  /*
  IPAddress local_IP(192,168,4,22);
  IPAddress gateway(192,168,4,9);
  IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(local_IP, gateway, subnet);//*/
  
  WiFi.softAP("ESP_con", "02012016");
  Serial.println("server connected !");
  
  Serial.print("IP:");
  Serial.println(WiFi.softAPIP());
  
  writeMsg("Please connect to :", 50);
  writeMsg((IpAddress2String(WiFi.softAPIP())+"/").c_str(), 50);
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}

String generatePsw()
{
  char psw[4];
  
  for(int i=0; i<3; i++)
    psw[i] = ((char)(random(26)%26) + 'A');
  psw[3] = '\0';

  return String(psw);
}

void happy_end()
{
  server.send(200, "text/html", htmlBody_thanks);
  
  Serial.println("Ids sent:"+id+"/"+psw);
  writeMsg("<---", 75);
  waitingForIDs = false;
}

void sad_end()
{
  server.send(200, "text/html", htmlBody_wrong);
}

void deleteIfAsked()
{
  if(server.hasArg("c1"))
     delIDs(0);
  if(server.hasArg("c2"))
     delIDs(1);
  if(server.hasArg("c3"))
     delIDs(2);
  if(server.hasArg("c4"))
     delIDs(3);
  if(server.hasArg("c5"))
     delIDs(4);
  if(server.hasArg("c6"))
     delIDs(5);
  if(server.hasArg("c7"))
     delIDs(6);
}
void resetIfAsked()
{
  if(server.hasArg("reset"))
     eraseALL();
}


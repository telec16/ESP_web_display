#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include "Texter.h"
#include "WifiServer.h"

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);
boolean waitingForIDs = true;

void setupWiFi();

char* htmlBody_root = "<h1>Please insert ids</h1><br/>\r\n"
  "<form method=\"post\" action=\"192.168.4.1/\">\r\n"
  "<label for=\"ssid\">SSID : </label>\r\n"
  "<input type=\"text\" name=\"ssid\" id=\"ssid\"/>\r\n"
  "<label for=\"psw\">MDP : </label>\r\n"
  "<input type=\"text\" name=\"psw\" id=\"psw\"/>\r\n"
  "<input id=\"button\" type=\"submit\" value=\"Envoyer\" />\r\n"
  "</form>\r\n";
char* htmlBody_thanks = "<h1>Thank you, redirection in progress...</h1>\r\n";
  
void handleRoot() {
  if (server.hasArg("ssid") && server.hasArg("psw")){
      server.send(200, "text/html", htmlBody_root);
      Serial.println("Ids sent");
      writeMsg("<---", 75);
      return;
  } else
    server.send(200, "text/html", htmlBody_root);
}
/*
char* htmlBody_on = "<h1>LED</h1><br/>\r\n"
  "LED is now on<br/>\r\n"
  "Click <a href=\"/led_off\">here</a> turn the LED on pin 2 OFF<br>\r\n";
char* htmlBody_off = "<h1>LED</h1><br/>\r\n"
  "LED is now on<br/>\r\n"
  "Click <a href=\"/led_on\">here</a> turn the LED on pin 2 ON<br>\r\n";

void handleLedOn(){
  server.send(200, "text/html", htmlBody_on);
  writeMsg("ON  ", 100);
}
void handleLedOff(){
  server.send(200, "text/html", htmlBody_off);
  writeMsg("OFF ", 100);
}*/

void createServerAndSetParams(String *ssid, String *password, int *option)
{  
  WiFiClient client;
  
  setupWiFi();
  server.on("/", handleRoot);
  server.onNotFound(handleRoot);
  server.begin();
  Serial.println("Server started");
  
  while(waitingForIDs){
    server.handleClient();
    /*while(!server.available());
    client = server.available();
    Serial.print("new client:");
    Serial.println(WiFi.softAPgetStationNum());
    while(!client.available()){
      delay(1);
    }
    // Read the first line of the request
    String request = client.readStringUntil('\n');
    Serial.print(request);
    Serial.println("here !");
    client.flush();
    
    // Match the request
    int value = LOW;
    if (request.indexOf("/LED=ON") != -1) {
      writeMsg("ON ", 100);
      value = HIGH;
    }
    else if (request.indexOf("/LED=OFF") != -1) {
      writeMsg("OFF", 100);
      value = LOW;
    }
    
    String s = "HTTP/1.1 200 OK\r\n";
    s += "Content-Type: text/html\r\n\r\n";
    s += "<!DOCTYPE HTML>\r\n<html>\r\n";
    s += "LED is now ";
    s += (value)?"on":"off";
    s += "<br><br>\r\n";
    s += "Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>\r\n";
    s += "Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>\r\n";
    s += "</html>\n";
  
    // Send the response to the client
    client.print(s);
    Serial.println(s);
    
    delay(10);
    Serial.println("Client disonnected");
    Serial.println("");//*/
  
  }
}

void setupWiFi()
{
  uint8_t mac_int[WL_MAC_ADDR_LENGTH];
  String mac = "";

  WiFi.mode(WIFI_AP);
  //WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("ESP_con", "02012016");
  Serial.println("connected !");
  
  /*WiFi.softAPmacAddress(mac_int);
  for (int i=0; i<WL_MAC_ADDR_LENGTH; i++)
    mac += String(mac_int[WL_MAC_ADDR_LENGTH-i], HEX) + ":";
  Serial.println("MAC:"+mac);*/
  
  Serial.print("IP:");
  Serial.println(WiFi.softAPIP());
  
  writeMsg(":D ", 50);
}


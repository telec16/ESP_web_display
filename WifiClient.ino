#include <ESP8266WiFi.h>
#include "Texter.h"
#include "WifiClient.h"

boolean connectToClient(int adempts, String ssid, String password)
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  
  writeMsg("    ", 1);
  while ((WiFi.status() != WL_CONNECTED) && (adempts-- != 0) && (digitalRead(BTN_UP) == 0)) {
    delay(500);
    Serial.print(".");
    writeMsg("Wait", 75);
  }
  Serial.print("\n");

  if(WiFi.status() != WL_CONNECTED)
    return false;

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  return true;
}


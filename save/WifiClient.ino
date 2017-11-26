#include <ESP8266WiFi.h>

boolean connectToClient(int adempts, String ssid, String password)
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  
  while ((WiFi.status() != WL_CONNECTED) && (adempts-- != 0)) {
    delay(500);
    Serial.print(".");
    writeMsg("Wait", 75);
  }

  if(WiFi.status() != WL_CONNECTED)
    return false;

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  return true;
}


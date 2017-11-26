#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

void createServerAndSetParams(String *ssid, String *password, int *option);
String IpAddress2String(const IPAddress& ipAddress);

#endif

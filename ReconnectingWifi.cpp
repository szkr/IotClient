// 
// 
// 

#include "ReconnectingWifi.h"
#include <ESP8266WiFi.h>

ReconnectingWifi::ReconnectingWifi(const char* ssid, const char* pass)
{
	this->ssid = ssid;
	this->pass = pass;
	WiFi.begin(ssid, pass);
	WiFi.setAutoConnect(true);
}


void ReconnectingWifi::loop()
{
	if (millis() - lastBlink2>1000)
	{
		lastBlink2 = millis();
		if (WiFi.status() != WL_DISCONNECTED && WiFi.status() != WL_CONNECTED) {
			Serial.println("Connecting to wifi");
			WiFi.begin(ssid, pass);
			WiFi.setAutoConnect(true);
		}
	}
}

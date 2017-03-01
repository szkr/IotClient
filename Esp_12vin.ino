#include "Updater.h"
#include "MotionNotifier.h"
#include "BrightnessReader.h"
#include "MotionSensorLamp.h"
#include "ReconnectingWifi.h"
#include "IotMessaging.h"
#include <Arduino.h>
#include <Esp.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID ""
#define WIFI_PASSWD ""
#define KEY ""
#define servIP ""
#define servFingerprint ""
#define FW_NAME ""
IotMessaging *messaging;
MotionSensorLamp*lamp;
ReconnectingWifi *wifi;
BrightnessReader  *brightnessReader;
MotionNotifier *motionNotifier;
Updater *updater;

void msgHandler(JsonObject& root)
{
	if (updater->processMessage(root, messaging)) return;
	if (lamp->processMessage(root, messaging)) return;
	if (!strcmp(root["command"], "getAmbientBrightness"))
	{
		StaticJsonBuffer<200> jsonBuffer2;
		JsonObject& root2 = jsonBuffer2.createObject();
		root2["ambientBrightness"] = brightnessReader->getAmbientBrightness();
		root2["id"] = root["id"];
		messaging->sendMessage(root2);
	}
	else if (!strcmp(root["command"], "restart"))
	{
		StaticJsonBuffer<200> jsonBuffer2;
		JsonObject& root2 = jsonBuffer2.createObject();
		root2["restart"] = true;
		root2["id"] = root["id"];
		messaging->sendMessage(root2);
		ESP.restart();
	}
}
void setup()
{
	Serial.begin(115200);
	wifi = new ReconnectingWifi(WIFI_SSID, WIFI_PASSWD);
	lamp = new MotionSensorLamp(16, 5);
	brightnessReader = new BrightnessReader();
	delay(300);

	Serial.printf("\n\nFW version: %s\n", FW_NAME);
	pinMode(2, OUTPUT);
	digitalWrite(2, HIGH);
	messaging = new IotMessaging(servIP, "/dev/lampa", KEY, FW_NAME);
	messaging->setMessageHandler(msgHandler);
	messaging->start();
	motionNotifier = new MotionNotifier(messaging);
	updater = new Updater(servIP, servFingerprint, FW_NAME);
	Serial.println("Setup completed");

}

//boolean blink = true;
//long lastBlink = 0;
void loop()
{
	//	if (millis() - lastBlink > 300)
	//	{
	//		blink = !blink;
	//		digitalWrite(2, blink);
	//		lastBlink = millis();
	//	}
	updater->loop();
	wifi->loop();
	delay(1);
	if (WiFi.status() == WL_CONNECTED)
		brightnessReader->loop();
	if (!lamp->loop()) {
		messaging->loop();
		motionNotifier->loop(lamp->getLastMotionDetectTime());
	}
}

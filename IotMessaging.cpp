// 
// 
// 

#include "IotMessaging.h"
#include <WebSocketsClient.h>
#include <WebSockets.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>
using namespace std::placeholders;

IotMessaging::IotMessaging(char* ip, char* path, char* key, const char* fwName)
{
	this->fwName = fwName;
	std::function<void(WStype_t type, uint8_t * payload, size_t length)> f = std::bind(&IotMessaging::webSocketEvent, this, _1, _2, _3);
	primaryIp = ip;
	this->path = path;
	this->key = key;
	client.onEvent(f);
	connected = false;
	retries = maxRetries - 1;
}

void IotMessaging::setMessageHandler(void(*msgRcvHandler)(JsonObject&))
{
	this->msgRcvHandler = msgRcvHandler;
}

void IotMessaging::start()
{
	lastMsgTime = millis();
	client.beginSSL(primaryIp.c_str(), 443, path.c_str());
}

void IotMessaging::loop()
{
	if (stopped) return;
	if (WiFi.isConnected() && millis() - reconnectBlockedTo > 120000)
		client.loop();
	if (connected && millis() - lastMsgTime > 30000)
	{
		connected = false;
		lastMsgTime = millis();
		Serial.println("Attempting to reconnect websockets");
		WiFi.disconnect();
	}
}

boolean IotMessaging::isConnected()
{
	return connected;
}

void IotMessaging::setMaxRetries(short m)
{
	maxRetries = m;
}

short IotMessaging::getMaxRetries()
{
	return maxRetries;
}

unsigned long IotMessaging::getReconnectWaitTime()
{
	return reconnectWaitTime;
}

void IotMessaging::setReconnectWaitTime(unsigned long m)
{
	reconnectBlockedTo = -m;
	reconnectWaitTime = m;
}

void IotMessaging::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
	switch (type) {
	case WStype_DISCONNECTED:
		retries++;
		if (retries > maxRetries)
			reconnectBlockedTo = millis();
		connected = false;
		Serial.println("Websockets disonnected");
		break;
	case WStype_CONNECTED:
		retries = 0;
		connected = true;
		lastMsgTime = millis();
		Serial.println("Websockets connected");
		{
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& root = jsonBuffer.createObject();
			root["fwName"] = fwName;
			sendMessage(root);
		}
		break;
	case WStype_TEXT: {
		lastMsgTime = millis();
		StaticJsonBuffer<400> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject((const char*)payload);
		if (!root.success() || !root.containsKey("command"))
			return;
		if (!strcmp(root["command"], "heartbeat"))
		{
			StaticJsonBuffer<200> jsonBuffer2;
			JsonObject& root2 = jsonBuffer2.createObject();
			root2["heartbeat"] = true;
			sendMessage(root2);
			return;
		}
		if (msgRcvHandler != NULL)
			msgRcvHandler(root);
		break;
	}

	default:break;
	}
}

void IotMessaging::sendMessage(JsonObject& root)
{
	if (!WiFi.isConnected() || !connected) return;
	root["key"] = key;
	char buffer[130];
	root.printTo(buffer, sizeof(buffer));
	client.sendTXT(buffer);
}

void IotMessaging::stop()
{
	client.disconnect();
	delete &client;
	stopped = true;
	Serial.println("Messaging stopped");
}


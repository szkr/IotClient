// IotMessaging.h

#ifndef _IOTMESSAGING_h
#define _IOTMESSAGING_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <WebSocketsClient.h>
#include <ArduinoJson.h>

class IotMessaging {
public:


	IotMessaging(char* ip, char* path, char* key, const char* fwName);
	void setMessageHandler(void(*msgRcvHandler)(JsonObject&));
	void start();
	void loop();
	boolean isConnected();
	void setMaxRetries(short m);
	short getMaxRetries();
	unsigned long getReconnectWaitTime();
	void setReconnectWaitTime(unsigned long m);
	void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);
	void sendMessage(JsonObject& root);
	void stop();
	WebSocketsClient client;
private:
	String primaryIp;
	String fallbackIp;
	String path;
	String key;
	void(*msgRcvHandler)(JsonObject&) = NULL;
	unsigned long lastMsgTime;
	boolean connected;
	short retries;
	short maxRetries = 3;
	unsigned long reconnectBlockedTo = -120000;
	unsigned long reconnectWaitTime = 120000;
	boolean stopped = false;
	const char* fwName;
};
#endif


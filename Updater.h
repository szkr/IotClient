// Updater.h
#ifndef _UPDATER_h
#define _UPDATER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "IotMessaging.h"

class Updater
{
public:
	Updater(String serverIp, String httpsFingerprint, const char* fwVersion);
	boolean processMessage(JsonObject& root, IotMessaging *messaging);
	void loop();
private:
	String serverIp;
	String httpsFingerprint;
	String fwname;
	boolean update;
	const char* fwVersion;
};

#endif


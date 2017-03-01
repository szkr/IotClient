// MotionSensorLamp.h

#ifndef _MOTIONSENSORLAMP_h
#define _MOTIONSENSORLAMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <ArduinoJson.h>
#include "IotMessaging.h"
enum LampMode { LampModeON, LampModeOFF, LampModeAUTO };

class MotionSensorLamp
{
public:
	MotionSensorLamp(int lampPin, int sensorPin);
	void setLampMode(LampMode mode);
	LampMode getLampMode();
	const char* getLampModeName();
	void setOnTimeAfterMove(unsigned long onTime);
	int getBrightness();
	void setBrightness(int brightness);
	unsigned long getOnTimeAfterMove();
	unsigned long getLastMotionDetectTime();
	boolean loop();
	boolean processMessage(JsonObject& root, IotMessaging *messaging);
private:
	unsigned long timeOnAfterMove = 15000;
	LampMode mode;
	int brightness;
	int lampPin;
	int sensorPin;
	int currentBrightness=0;
	unsigned long lastMotionDetect = 0;
};

#endif


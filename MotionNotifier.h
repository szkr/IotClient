// MotionNotifier.h

#ifndef _MOTIONNOTIFIER_h
#define _MOTIONNOTIFIER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "IotMessaging.h"

class MotionNotifier
{
public:
	MotionNotifier(IotMessaging *messaging);
	void loop(unsigned long lastMoveTime);
private:
	IotMessaging *messaging;
	unsigned long lastSend;
	unsigned long delayBetweenSend;
};

#endif


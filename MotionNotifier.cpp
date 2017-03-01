// 
// 
// 

#include "MotionNotifier.h"


MotionNotifier::MotionNotifier(IotMessaging* messaging)
{
	this->messaging = messaging;
	lastSend = 5000;
	delayBetweenSend = 10000;
}

void MotionNotifier::loop(unsigned long lastMoveTime)
{
	if (millis() - lastSend > delayBetweenSend && millis() - lastMoveTime < 100)
	{
		lastSend = millis();
		StaticJsonBuffer<200> jsonBuffer2;
		JsonObject& root2 = jsonBuffer2.createObject();
		root2["motionDetect"] = true;
		messaging->sendMessage(root2);
	}
}

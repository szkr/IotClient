// 
// 
// 

#include "MotionSensorLamp.h"


MotionSensorLamp::MotionSensorLamp(int lampPin, int sensorPin)
{
	mode = LampModeAUTO;
	brightness = 1023;
	this->lampPin = lampPin;
	this->sensorPin = sensorPin;
}


boolean MotionSensorLamp::processMessage(JsonObject& root, IotMessaging* messaging)
{
	if (!strcmp(root["command"], "setMode") || !strcmp(root["command"], "getMode"))
	{
		if (!strcmp(root["command"], "setMode"))
		{
			if (!strcmp(static_cast<const char*>(root["mode"]), "ON"))
				setLampMode(LampModeON);
			else if (!strcmp(static_cast<const char*>(root["mode"]), "OFF"))
				setLampMode(LampModeOFF);
			else
				setLampMode(LampModeAUTO);

		}
		StaticJsonBuffer<200> jsonBuffer2;
		JsonObject& root2 = jsonBuffer2.createObject();
		root2["mode"] = getLampModeName();
		root2["id"] = root["id"];
		messaging->sendMessage(root2);
		return true;
	}

	if (!strcmp(root["command"], "setBrightness") || !strcmp(root["command"], "getBrightness"))
	{
		if (!strcmp(root["command"], "setBrightness"))
			setBrightness(root["brightness"]);
		StaticJsonBuffer<200> jsonBuffer2;
		JsonObject& root2 = jsonBuffer2.createObject();
		root2["brightness"] = getBrightness();
		root2["id"] = root["id"];
		messaging->sendMessage(root2);
		return true;
	}
	if (!strcmp(root["command"], "setTimeOnAfterMove") || !strcmp(root["command"], "getTimeOnAfterMove"))
	{
		if (!strcmp(root["command"], "setTimeOnAfterMove"))
			setOnTimeAfterMove(root["timeOnAfterMove"]);
		StaticJsonBuffer<200> jsonBuffer2;
		JsonObject& root2 = jsonBuffer2.createObject();
		root2["timeOnAfterMove"] = getOnTimeAfterMove();
		root2["id"] = root["id"];
		messaging->sendMessage(root2);
		return true;
	}


	return false;
}

void MotionSensorLamp::setLampMode(LampMode mode)
{
	this->mode = mode;
	if (mode == LampModeON) {
		analogWrite(lampPin, brightness);
		digitalWrite(lampPin, HIGH);
		//analogWrite(2, HIGH);
	}
	else if (mode == LampModeOFF) {
		analogWrite(lampPin, 0);
		//analogWrite(2, LOW);
		digitalWrite(lampPin, LOW);
	}
	else {
		digitalWrite(lampPin, HIGH);
		analogWrite(lampPin, currentBrightness / 2);
	}
}

LampMode MotionSensorLamp::getLampMode()
{
	return mode;
}

const char* MotionSensorLamp::getLampModeName()
{
	if (mode == LampModeON)
		return "ON";
	if (mode == LampModeOFF)
		return "OFF";
	return "AUTO";
}


void MotionSensorLamp::setOnTimeAfterMove(unsigned long onTime)
{
	timeOnAfterMove = onTime;
	if (timeOnAfterMove > 7200000) timeOnAfterMove = 7200000;

}

int MotionSensorLamp::getBrightness()
{
	return brightness;
}

void MotionSensorLamp::setBrightness(int brightness)
{
	if (brightness < 0) brightness = 0;
	else if (brightness > 1023) brightness = 1023;
	this->brightness = brightness;
	if (mode == LampModeON)
		analogWrite(lampPin, brightness);

}

unsigned long MotionSensorLamp::getOnTimeAfterMove()
{
	return timeOnAfterMove;
}

unsigned long MotionSensorLamp::getLastMotionDetectTime()
{
	return lastMotionDetect;
}

//returns true if the lamp is dimming now to not execute any other commands taking time
boolean MotionSensorLamp::loop()
{
	if (digitalRead(5) == HIGH)
		lastMotionDetect = millis();
	if (mode != LampModeAUTO) return false;
	if (millis() - lastMotionDetect < timeOnAfterMove && currentBrightness < 2046) {
		if (currentBrightness > 600)
			currentBrightness++;
		currentBrightness++;
	}
	else if (millis() - lastMotionDetect >= timeOnAfterMove && currentBrightness > 0) {
		if (currentBrightness > 600)
			currentBrightness--;
		currentBrightness--;
	}

	analogWrite(lampPin, currentBrightness / 2);
	return currentBrightness > 0 && currentBrightness < 2046;
}

// BrightnessReader.h

#ifndef _BRIGHTNESSREADER_h
#define _BRIGHTNESSREADER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
class BrightnessReader
{
public:
	BrightnessReader();
	void loop();
	float getAmbientBrightness();
private:
	short brightnessReads[256];
	unsigned char counter = 0;
};

#endif


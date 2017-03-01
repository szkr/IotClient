// 
// 
// 

#include "BrightnessReader.h"

BrightnessReader::BrightnessReader()
{
	short currentBrightness = analogRead(A0);
	for (int i = 0; i < 256; i++)
		brightnessReads[i] = currentBrightness;
}

float BrightnessReader::getAmbientBrightness()
{
	unsigned int brightnessSum = 0;
	for (int i = 0; i < 256; i++)
		brightnessSum += brightnessReads[i];
	return brightnessSum / 2618.88;
}

void BrightnessReader::loop()
{
	brightnessReads[counter++] = analogRead(A0);
}


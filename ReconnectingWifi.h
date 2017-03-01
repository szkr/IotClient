// ReconnectingWifi.h

#ifndef _RECONNECTINGWIFI_h
#define _RECONNECTINGWIFI_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
	class ReconnectingWifi
	{
	public:
		ReconnectingWifi(const char* ssid, const char* pass);
		void loop();
	private:
		unsigned long lastBlink2 =-2000;
		const char* ssid;
		const char* pass;
	};

#endif


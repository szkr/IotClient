// 
// 
// 
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include "Updater.h"

Updater::Updater(String serverIp, String httpsFingerprint, const char* fwVersion)
{
	this->serverIp = serverIp + "/fw/";
	this->httpsFingerprint = httpsFingerprint;
	update = false;
	this->fwVersion = fwVersion;
}

boolean Updater::processMessage(JsonObject& root, IotMessaging* messaging)
{
	if (!strcmp(root["command"], "getFWVersion"))
	{
		StaticJsonBuffer<200> jsonBuffer2;
		JsonObject& root2 = jsonBuffer2.createObject();
		root2["version"] = fwVersion;
		root2["id"] = root["id"];
		messaging->sendMessage(root2);
		return true;
	}
	if (strcmp(root["command"], "updateFirmware")) return false;
	fwname = String(static_cast<const char*>(root["firmwareName"]));
	update = true;
	messaging->stop();
	return true;
}
void Updater::loop()
{
	if (!update) return;
	update = false;
	ESPhttpUpdate.rebootOnUpdate(true);
	t_httpUpdate_return ret = ESPhttpUpdate.update("https://" + serverIp + fwname, "", httpsFingerprint);
	switch (ret) {
	case HTTP_UPDATE_FAILED:
		Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
		break;

	case HTTP_UPDATE_NO_UPDATES:
		Serial.println("HTTP_UPDATE_NO_UPDATES");
		break;

	case HTTP_UPDATE_OK:
		Serial.println("HTTP_UPDATE_OK");
		break;
		
	}
	ESP.restart();
}

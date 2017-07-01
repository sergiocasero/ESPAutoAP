#ifndef ESPAutoAP_h
#define ESPAutoAP_h

#include "Arduino.h"
#include "EEPROM.h"

#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "WiFiClient.h"

class ESPAutoAP
{
public:
	/*
		Call this method in the setup() main method
		With this method you can pass to the library custom ssid and password for ap mode
		ssidAp -> The ssid you want
		passwordAp -> The password you want
		debug -> enable this flag to see the logs
	*/
	ESPAutoAP(const char* ssidAp, const char* passwordAp, bool debug);

	/*
		Call this method in the setup() main method
	*/
	void setup();

	/*
		This method will check for calls
	*/
	void loop();

private:
	const char* ssidAp;

	const char* passwordAp;
};

#endif
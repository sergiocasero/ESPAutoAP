#include "ESPAutoAP.h"

ESP8266WebServer server = ESP8266WebServer(8080);

int ssidStartAddress = 0;

int ssidEndAddress = 31;

int passwordStartAddress = 32;

int passwordEndAddress = 95;

bool debug = false;

ESPAutoAP::ESPAutoAP(const char* ssidAp, const char* passwordAp, bool enableDebug)	{
	this->ssidAp = ssidAp;
	this->passwordAp = passwordAp;
	debug = enableDebug;

	if(debug) {
		Serial.println("AutoAP instanciated");
		Serial.print("SSID: ");
		Serial.write(this->ssidAp);
		Serial.println();
		Serial.print("PASSWORD: ");
		Serial.write(this->passwordAp);
		Serial.println();
	}
}

void ESPAutoAP::setup() {
	if(debug) Serial.println("Initialize EEPROM");

	EEPROM.begin(512);
	delay(10);

	if(debug) Serial.println("Reading SSID");

	String ssid;
	for (int i = ssidStartAddress; i <= ssidEndAddress; i++) {
		ssid += char(EEPROM.read(i));
	}

	if(debug) Serial.println("Reading password");

	String password;
	for (int i = passwordStartAddress; i <= passwordEndAddress; i++) {
		password += char(EEPROM.read(i));
	}

	if(debug) Serial.println("SSID: " + ssid);
 	if(debug) Serial.println("Password: " + password);

 	bool staMode = false;

 	if(!ssid.equals("")){
 		WiFi.mode(WIFI_STA);
  		WiFi.begin(ssid.c_str(), password.c_str());

		if(debug) Serial.println("Trying to connect");

		int i = 0;
		int retries = 100;
		while (WiFi.status() != WL_CONNECTED && i < retries) {
			delay(500);
			if(debug) Serial.print(".");
			i++;
		}

		if(i < retries) {
			staMode = true;
		}
 	}

	if (!staMode) {
		if(debug) Serial.println("I don't have the credentials!!"); 
		if(debug) Serial.println("Waking up the soft AP");

		WiFi.mode(WIFI_AP);
		WiFi.softAP(ssidAp, passwordAp);
	} else {
		if(debug) Serial.println("Connected!! my IP is " + WiFi.localIP());
	}

	if(debug) Serial.println("Starting the server...");
	delay(500);
	server.begin();
	delay(250);

	server.on ( "/ap/configure", HTTP_POST, []{
		String ssid = server.arg("ssid");
		String password = server.arg("password");

		if(debug) Serial.println("writing eeprom ssid:");
		for (int i = ssidStartAddress; i < ssid.length(); ++i) {
			EEPROM.write(i, ssid[i]);
			if(debug) Serial.print(ssid[i]); 
		}

		if(debug) Serial.println();

		Serial.println("writing eeprom pass:"); 
		for (int i = 0; i < password.length(); ++i) {
			EEPROM.write(passwordStartAddress + i, password[i]);
			if(debug) Serial.print(password[i]); 
		}    

		if(debug) Serial.println();

		EEPROM.commit();

		server.send(200, "application/json", "{\"configured\": \"ok\"}");

		ESP.restart();
	});
	server.on ( "/ap/reset", HTTP_POST, []{
		if(debug) Serial.println("Reseting all configurations...");

		for (int i = ssidStartAddress; i <= passwordEndAddress; ++i) {
			EEPROM.write(i, 0);
		}
		EEPROM.commit();

		server.send(200, "application/json", "{\"reset\": \"ok\"}");

		ESP.restart();
	});
}

void ESPAutoAP::loop() {
	server.handleClient();
}
# ESPAutoAP
This library provides you auto-connection to your WiFi network, and if the ESP doesn't have the credentials, it will create custom AP, and you only need to call 1 Web service!!

# How use this library
```processing
#include <ESPAutoAP.h>

// Initialize the ESP, with the AP SSID, password, and if you want to see the debug logs, set the last parameter to true
ESPAutoAP autoAP("TEST", "testpass", true);

void setup() {
  Serial.begin(115200);
  
  // call to setup()
  autoAP.setup();
}

void loop() {

  // call to loop()
  autoAP.loop();
}
```

That's all ¯\_(ツ)_/¯

# How can I configure my home network?

This step is really easy, the only steps you need are:
1. Connect to the temporal AP (in the example case-> TEST:testpass )
2. Make a request like this ->
```http
POST 192.168.4.1:8080/ap/configure?ssid=YOUR_HOME_SSID&password=YOUR_HOME_PASSWORD
```

IMPORTANT: Encode the ssid and password with URLEncode

That's all ¯\_(ツ)_/¯

# How can I reset the ESP state?
Simple, make a request like this ->
```http
POST 192.168.4.1:8080/ap/reset
```

That's all ¯\_(ツ)_/¯

# How it works?

The lifecycle is simple:

1. Try to get the home credentials from the EEPROM
1.1 If the ESP have them, connect to the network
1.2 Else, wake up the custom AP


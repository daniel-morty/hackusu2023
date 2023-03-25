#include <WiFi.h>
#include "motor.h"
#include "web_server.h"
#include <FastLED.h>
#include "led.h"


CRGB leds[NUM_LEDS];

const char *ssid = "TractorHostG";
const char *password = "TractorHost";
extern bool do_autopilot;


void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  setupMotors();
  setupWebServer();

  xTaskCreatePinnedToCore(motor1Task, "Motor1Task", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(motor2Task, "Motor2Task", 1000, NULL, 1, NULL, 1);

	FastLED.addLeds<SK6812, DATA_PIN>(leds, NUM_LEDS); //TODO Check led type

}

void loop() {
  wsProcess();
  vTaskDelay(100);
  if(do_autopilot){
	controlMotors(-640, -305);
  }
}

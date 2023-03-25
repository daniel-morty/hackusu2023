#include "web_server.h"
#include "webpage.h"
#include "motor.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
extern bool do_autopilot;

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
	Serial.println(WS_EVT_DATA);
  if (type == WS_EVT_DATA) {
	if (strncmp((const char *)data, "AUTOPILOT:", 10) == 0) {
	  bool autopilot = data[10] == '1';
	  // Implement your autopilot logic here
		Serial.println("start autopilot");
		if(!do_autopilot){
			enable_autopilot();
		}else{
			disable_autopilot();
		}
	}
	else{
		int16_t x = (data[0] << 8) | data[1];
		int16_t y = (data[2] << 8) | data[3];
		Serial.printf("x: %d, y: %d", x, y);

		controlMotors(x, y);
	}
  }
}


void setupWebServer() {
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", indexHtml);
  });
  server.begin();
}

void wsProcess()
{
  ws.cleanupClients();
}

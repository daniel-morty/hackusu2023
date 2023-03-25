#include <ESPAsyncWebServer.h>

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void setupWebServer();
void wsProcess();
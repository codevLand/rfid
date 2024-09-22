#include "Api.h"
#include "Peripherals.h"

Api::Api(API_SERVER api) {
  api.port = SERVER_PORT
  AsyncWebServer server(api.port);    // 192.168.1.25:80 OR localhost:80
  server.begin();
}

void routes() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) { onIndex(); });
  server.on("/unlock", HTTP_GET, [](AsyncWebServerRequest* request) { onUnlockGate(); } );
  server.on("/register", HTTP_GET, [](AsyncWebServerRequest* request) { onSignup } );
}

void onIndex() {
  String html = "<html><body><h1>Gate Control<h1>";
  html += "<button onclick=\"location.href='/unlock'\">Unlock Gate</button>";
  html += "<button onclick=\"location.href='/register'\">Register Card</button>";
  html += "</body></html>";

  request->send(200, "text/html", html);
  }

void onUnlockGate() {
  Periperals allowGatePassage();
  request->send(200, "text/html", "Gate Unlocked");
}

void onSignup() {
  Periperals readRFID();
  request->send(200, "text/html", "Register Card");
}
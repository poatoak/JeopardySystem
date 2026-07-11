#include <esp_now.h>
#include <WiFi.h>

bool[] lockouts = {false, false, false, false, false, false, false, false};
uint8_t[] addresses {{}, {}, {}, {}, {}, {}, {}, {}};

typedef struct Message { 
  int ID;
} Message;

typedef struct Response { 
  bool lockout;
} Response;

void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  Message msg;
  memcpy(&msg, data, sizeof(msg));
  int ID = msg.ID;

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, addresses[ID], 6);
  if (!esp_now_is_peer_exist(addresses[ID])) {
    esp_now_add_peer(&peer);
  }

  Response response;
  response.lockout = lockouts[ID];
  esp_now_send((uint8_t *)mac, (uint8_t *)&response, sizeof(response));
}

void setup() {

}

void loop() {
  
}
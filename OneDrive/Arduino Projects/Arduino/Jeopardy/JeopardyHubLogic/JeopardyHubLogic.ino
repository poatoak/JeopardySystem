#include <esp_now.h>
#include <WiFi.h>

bool lockouts[] = {false, false, false, false, false, false, false, false};

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

  Serial.println("BUZZ " + String(ID));
  if (!esp_now_is_peer_exist(mac)) {
    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, mac, 6);
    esp_now_add_peer(&peer);
  }

  Response response;
  response.lockout = lockouts[ID];
  esp_now_send((uint8_t *)mac, (uint8_t *)&response, sizeof(response));
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(onReceive);
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("LOCK ")) {
      int ID = cmd.substring(5).toInt();
      lockouts[ID] = true;
    } else if (cmd.startsWith("UNLOCK ")) {
      int ID = cmd.substring(7).toInt();
      lockouts[ID] = false;
    } else if (cmd == "RESET") {
      for (int i = 0; i < 8; i++) {
        lockouts[i] = false;
      }
    }
  }
}
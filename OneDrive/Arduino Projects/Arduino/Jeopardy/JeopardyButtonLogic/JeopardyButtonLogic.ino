#include <esp_now.h>
#include <WiFi.h>

int buttonPin = 23;
int callPin = 18;
bool lockedout = true;
int ID = 1;

uint8_t hubAddress[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};

typedef struct Message {
  int ID;
} Message;

typedef struct Response {
  bool lockedout;
} Response;

Response response;
volatile bool responseReceived = false;

void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  memcpy(&response, data, sizeof(response));
  responseReceived = true;
}

void setup() {
  //Setup
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(callPin, OUTPUT);

  WiFi.mode(WIFI_STA);
  esp_now_init();

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, hubAddress, 6);
  esp_now_add_peer(&peer);

  esp_now_register_recv_cb(onReceive);
  //End Setup

  // Send ID to Hub
  while (!responseReceived) {
    Message msg;
    msg.ID = ID;
    esp_now_send(hubAddress, (uint8_t *)&msg, sizeof(msg));
    delay(7); //waits 7millis before looping for response
  }

  lockedout = response.lockedout; //Sets lockout to given value

  //Button Logic
    if (!lockedout) {
      //Pushes power to the pcb, closing the circuit and powering the button
      digitalWrite(callPin, HIGH);
      delay(20);
      digitalWrite(callPin, LOW);
    }

  //Sleep Call
  esp_sleep_enable_ext0_wakeup(23, HIGH);
  esp_deep_sleep_start();
}

void loop() {
}
#include <WiFi.h>
#include <esp_now.h>

uint8_t broadcastAddress[] = {0x44, 0x17, 0x93, 0x8F, 0x4E, 0x38};

long int rotValue=0, swValue=0;
uint8_t state=0;

#define ROTARY_CLK 2
#define ROTARY_DT 4
#define ROTARY_SW 6

portMUX_TYPE gpioMux = portMUX_INITIALIZER_UNLOCKED;

typedef struct struct_message {
  int action;  
} struct_message;

struct_message myMessage;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void dataSentCallback(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("isrSWAll ");
  Serial.print(swValue);
  
  Serial.print(" rotValue ");
  Serial.print(rotValue);
  
  Serial.print(" action ");
  Serial.println(myMessage.action);
  
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void IRAM_ATTR isrAB() {
  portENTER_CRITICAL_ISR(&gpioMux);
  
  uint8_t s = state & 3;

  if (digitalRead(ROTARY_CLK)) s |= 8;
  if (digitalRead(ROTARY_DT)) s |= 4;

  switch (s) {
    case 1: case 14:
      rotValue++;
      myMessage.action = 1;
      esp_now_send(broadcastAddress, (uint8_t *) &myMessage, sizeof(myMessage));
      break;
    case 2: case 13:
      myMessage.action = 2;
      esp_now_send(broadcastAddress, (uint8_t *) &myMessage, sizeof(myMessage));
      rotValue--; 
      break;
    default:
      break;
  }
  
  state = (s >> 2);
  
  portEXIT_CRITICAL_ISR(&gpioMux);
}


void IRAM_ATTR isrSWAll() {
  portENTER_CRITICAL_ISR(&gpioMux);

  myMessage.action = 3;
  esp_now_send(broadcastAddress, (uint8_t *) &myMessage, sizeof(myMessage));
  swValue++;
  
  portEXIT_CRITICAL_ISR(&gpioMux);
}

void setup(){
  Serial.begin(115200);
  
  pinMode(ROTARY_CLK, INPUT_PULLUP);
  pinMode(ROTARY_DT, INPUT_PULLUP);
  pinMode(ROTARY_SW, INPUT_PULLUP);

  attachInterrupt(ROTARY_CLK, isrAB, CHANGE);
  attachInterrupt(ROTARY_DT, isrAB, CHANGE);
  attachInterrupt(ROTARY_SW, isrSWAll, CHANGE);
  
   WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(dataSentCallback);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop(){

}

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "Message.h"

uint8_t broadcastAddress[] = {0x44, 0x17, 0x93, 0x8F, 0x4E, 0x38};

esp_now_peer_info_t peerInfo;

void send_message(struct_message message) {
  esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
}

void setup_communication() {
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  //esp_now_register_send_cb(dataSentCallback);
}
